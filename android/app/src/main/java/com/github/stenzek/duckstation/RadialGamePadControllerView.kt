package com.github.stenzek.duckstation

import android.content.Context
import android.util.AttributeSet
import android.view.Gravity
import android.view.MotionEvent
import android.widget.FrameLayout
import com.swordfish.radialgamepad.library.RadialGamePad
import com.swordfish.radialgamepad.library.config.ButtonConfig
import com.swordfish.radialgamepad.library.config.CrossConfig
import com.swordfish.radialgamepad.library.config.PrimaryDialConfig
import com.swordfish.radialgamepad.library.config.RadialGamePadConfig
import com.swordfish.radialgamepad.library.config.SecondaryDialConfig
import com.swordfish.radialgamepad.library.event.Event
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.SupervisorJob
import kotlinx.coroutines.cancel
import kotlinx.coroutines.flow.collect
import kotlinx.coroutines.launch

class RadialGamePadControllerView @JvmOverloads constructor(
    context: Context,
    attrs: AttributeSet? = null,
    defStyle: Int = 0
) : FrameLayout(context, attrs, defStyle) {

    private var mControllerIndex = 0
    private var mControllerType  = ""

    private val scope = CoroutineScope(Dispatchers.Main + SupervisorJob())

    // Toggle state: tracks which toggle buttons are currently pressed
    private val toggleButtonIds = LocalPadConfig.toggleButtons
    private val toggleState = mutableMapOf<Int, Boolean>()

    override fun onDetachedFromWindow() {
        super.onDetachedFromWindow()
        scope.cancel()
    }

    fun init(controllerIndex: Int, controllerType: String, viewType: String) {
        mControllerIndex = controllerIndex
        mControllerType  = controllerType

        removeAllViews()

        val hasAnalog = controllerType == "AnalogController" && viewType != "digital"
        setupPad(LocalPadConfig.leftPad,  hasAnalog, Gravity.START)
        setupPad(LocalPadConfig.rightPad, hasAnalog, Gravity.END)
    }

    fun updateOrientation() = Unit

    // -------------------------------------------------------------------------
    // Build a RadialGamePadConfig from a PadConfig data object
    // -------------------------------------------------------------------------

    private fun buildPadConfig(config: PadConfig, hasAnalog: Boolean): RadialGamePadConfig {
        val secondaryDials = mutableListOf<SecondaryDialConfig>()

        for (dial in config.secondaryDials) {
            when (dial) {
                is PadDial.Empty -> secondaryDials += SecondaryDialConfig.Empty(
                    dial.index, dial.spread, dial.scale, dial.distance
                )
                is PadDial.Button -> secondaryDials += SecondaryDialConfig.SingleButton(
                    dial.index, dial.scale, dial.distance,
                    ButtonConfig(id = dial.id, label = dial.label)
                )
                is PadDial.Stick -> if (hasAnalog) secondaryDials += SecondaryDialConfig.Stick(
                    index    = dial.index,
                    spread   = dial.spread,
                    scale    = dial.scale,
                    distance = dial.distance,
                    id       = dial.id
                )
            }
        }

        val primaryDial = when (val p = config.primaryDial) {
            is PrimaryDialDef.Cross -> PrimaryDialConfig.Cross(CrossConfig(id = p.id))
            is PrimaryDialDef.Buttons -> PrimaryDialConfig.PrimaryButtons(
                dials = p.dials.map { ButtonConfig(id = it.id, label = it.label) }
            )
        }

        return RadialGamePadConfig(
            sockets        = config.socketCount,
            primaryDial    = primaryDial,
            secondaryDials = secondaryDials
        )
    }

    private fun setupPad(config: PadConfig, hasAnalog: Boolean, gravity: Int) {
        val pad = RadialGamePad(buildPadConfig(config, hasAnalog), LocalPadConfig.PAD_SIZE, context)
        addView(pad, LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.MATCH_PARENT).apply {
            this.gravity = gravity or Gravity.CENTER_VERTICAL
        })
        scope.launch {
            pad.events().collect { handleEvent(it) }
        }
    }

    // -------------------------------------------------------------------------
    // Event handling
    // -------------------------------------------------------------------------

    private fun handleEvent(event: Event) {
        when (event) {
            is Event.Button    -> handleButton(event)
            is Event.Direction -> handleDirection(event)
        }
    }

    private fun handleButton(event: Event.Button) {
        val code = idToButtonCode(event.id) ?: return

        if (event.id in toggleButtonIds) {
            // Toggle on ACTION_DOWN only
            if (event.action == MotionEvent.ACTION_DOWN) {
                val newState = !(toggleState[event.id] ?: false)
                toggleState[event.id] = newState
                AndroidHostInterface.getInstance().setControllerButtonState(mControllerIndex, code, newState)
            }
        } else {
            val pressed = event.action == MotionEvent.ACTION_DOWN
            AndroidHostInterface.getInstance().setControllerButtonState(mControllerIndex, code, pressed)
        }
    }

    private fun handleDirection(event: Event.Direction) {
        when (event.id) {
            PsxIds.DPAD -> {
                val upCode    = AndroidHostInterface.getControllerButtonCode(mControllerType, "Up")
                val downCode  = AndroidHostInterface.getControllerButtonCode(mControllerType, "Down")
                val leftCode  = AndroidHostInterface.getControllerButtonCode(mControllerType, "Left")
                val rightCode = AndroidHostInterface.getControllerButtonCode(mControllerType, "Right")

                val t = 0.5f
                setButton(upCode,    event.yAxis < -t)
                setButton(downCode,  event.yAxis >  t)
                setButton(leftCode,  event.xAxis < -t)
                setButton(rightCode, event.xAxis >  t)
            }
            PsxIds.LEFT_STICK -> {
                val xCode = AndroidHostInterface.getControllerAxisCode(mControllerType, "LeftX")
                val yCode = AndroidHostInterface.getControllerAxisCode(mControllerType, "LeftY")
                if (xCode >= 0) AndroidHostInterface.getInstance()
                    .setControllerAxisState(mControllerIndex, xCode, event.xAxis)
                if (yCode >= 0) AndroidHostInterface.getInstance()
                    .setControllerAxisState(mControllerIndex, yCode, event.yAxis)
            }
            PsxIds.RIGHT_STICK -> {
                val xCode = AndroidHostInterface.getControllerAxisCode(mControllerType, "RightX")
                val yCode = AndroidHostInterface.getControllerAxisCode(mControllerType, "RightY")
                if (xCode >= 0) AndroidHostInterface.getInstance()
                    .setControllerAxisState(mControllerIndex, xCode, event.xAxis)
                if (yCode >= 0) AndroidHostInterface.getInstance()
                    .setControllerAxisState(mControllerIndex, yCode, event.yAxis)
            }
        }
    }

    private fun setButton(code: Int, pressed: Boolean) {
        if (code < 0) return
        AndroidHostInterface.getInstance().setControllerButtonState(mControllerIndex, code, pressed)
    }

    private fun idToButtonCode(id: Int): Int? {
        val name = when (id) {
            PsxIds.CROSS     -> "Cross"
            PsxIds.CIRCLE    -> "Circle"
            PsxIds.SQUARE    -> "Square"
            PsxIds.TRIANGLE  -> "Triangle"
            PsxIds.L1        -> "L1"
            PsxIds.L2        -> "L2"
            PsxIds.R1        -> "R1"
            PsxIds.R2        -> "R2"
            PsxIds.SELECT    -> "Select"
            PsxIds.START     -> "Start"
            PsxIds.L3        -> "L3"
            PsxIds.R3        -> "R3"
            else             -> return null
        }
        val code = AndroidHostInterface.getControllerButtonCode(mControllerType, name)
        return if (code >= 0) code else null
    }
}