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

    companion object {
        private const val ID_DPAD         = 0
        private const val ID_LEFT_STICK   = 1
        private const val ID_RIGHT_STICK  = 2

        private const val ID_CIRCLE       = 10
        private const val ID_CROSS        = 11
        private const val ID_SQUARE       = 12
        private const val ID_TRIANGLE     = 13

        private const val ID_L1           = 20
        private const val ID_L2           = 21
        private const val ID_R1           = 22
        private const val ID_R2           = 23

        private const val ID_SELECT       = 30
        private const val ID_START        = 31
        private const val ID_L3           = 32
        private const val ID_R3           = 33
    }

    private var mControllerIndex = 0
    private var mControllerType  = ""

    private val scope = CoroutineScope(Dispatchers.Main + SupervisorJob())

    override fun onDetachedFromWindow() {
        super.onDetachedFromWindow()
        scope.cancel()
    }

    fun init(controllerIndex: Int, controllerType: String, viewType: String) {
        mControllerIndex = controllerIndex
        mControllerType  = controllerType

        removeAllViews()

        val hasAnalog = controllerType == "AnalogController" && viewType != "digital"
        setupLeftPad(hasAnalog)
        setupRightPad(hasAnalog)
    }

    fun updateOrientation() = Unit

    // -------------------------------------------------------------------------
    // Socket index map (8 sockets, clockwise from top):
    //         0
    //     7       1
    //   6           2
    //     5       3
    //         4
    //
    // Left pad:  D-pad center, shoulder buttons across the top, Select top-center
    // Right pad: Face buttons center, shoulder buttons across the top, Start top-center
    // -------------------------------------------------------------------------

    private fun buildLeftPadConfig(hasAnalog: Boolean): RadialGamePadConfig {
        val secondaryDials = mutableListOf<SecondaryDialConfig>()

        secondaryDials += SecondaryDialConfig.Empty(2, 1, 0.5f, 2f)

        secondaryDials += SecondaryDialConfig.SingleButton(
            2, 1f, 0f,
            ButtonConfig(id = ID_SELECT, label = "SEL")
        )
        secondaryDials += SecondaryDialConfig.SingleButton(
            3, 1f, 0f,
            ButtonConfig(id = ID_L1, label = "L1")
        )
        secondaryDials += SecondaryDialConfig.SingleButton(
            4, 1f, 0f,
            ButtonConfig(id = ID_L2, label = "L2")
        )

        if (hasAnalog) {
            secondaryDials += SecondaryDialConfig.Empty(8, 1, 1f, 0f)
            secondaryDials += SecondaryDialConfig.Stick(
                index    = 9,
                spread   = 2,
                scale    = 2.2f,
                distance = 0.1f,
                id       = ID_LEFT_STICK
            )
        }

        return RadialGamePadConfig(
            sockets        = 12,
            primaryDial    = PrimaryDialConfig.Cross(CrossConfig(id = ID_DPAD)),
            secondaryDials = secondaryDials
        )
    }

    private fun buildRightPadConfig(hasAnalog: Boolean): RadialGamePadConfig {
        val secondaryDials = mutableListOf<SecondaryDialConfig>()

        secondaryDials += SecondaryDialConfig.Empty(4, 1, 0.5f, 2f)

        secondaryDials += SecondaryDialConfig.SingleButton(
            2, 1f, 0f,
            ButtonConfig(id = ID_R1, label = "R1")
        )
        secondaryDials += SecondaryDialConfig.SingleButton(
            3, 1f, 0f,
            ButtonConfig(id = ID_R2, label = "R2")
        )
        secondaryDials += SecondaryDialConfig.SingleButton(
            4, 1f, 0f,
            ButtonConfig(id = ID_START, label = "START")
        )

        if (hasAnalog) {
            secondaryDials += SecondaryDialConfig.Empty(8, 1, 1f, 0f)
            secondaryDials += SecondaryDialConfig.Stick(
                index    = 9,
                spread   = 2,
                scale    = 2.2f,
                distance = 0.1f,
                id       = ID_RIGHT_STICK
            )
        }

        val faceButtons = PrimaryDialConfig.PrimaryButtons(
            dials = listOf(
                ButtonConfig(id = ID_CIRCLE,   label = "○"),
                ButtonConfig(id = ID_TRIANGLE, label = "△"),
                ButtonConfig(id = ID_SQUARE,   label = "□"),
                ButtonConfig(id = ID_CROSS,    label = "✕")
            )
        )

        return RadialGamePadConfig(
            sockets        = 12,
            primaryDial    = faceButtons,
            secondaryDials = secondaryDials
        )
    }

    private fun setupLeftPad(hasAnalog: Boolean) {
        val pad = RadialGamePad(buildLeftPadConfig(hasAnalog), 25f, context)
        addView(pad, LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.MATCH_PARENT).apply {
            gravity = Gravity.START or Gravity.CENTER_VERTICAL
        })
        scope.launch {
            pad.events().collect { handleEvent(it) }
        }
    }

    private fun setupRightPad(hasAnalog: Boolean) {
        val pad = RadialGamePad(buildRightPadConfig(hasAnalog), 25f, context)
        addView(pad, LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.MATCH_PARENT).apply {
            gravity = Gravity.END or Gravity.CENTER_VERTICAL
        })
        scope.launch {
            pad.events().collect { handleEvent(it) }
        }
    }

    private fun handleEvent(event: Event) {
        when (event) {
            is Event.Button    -> handleButton(event)
            is Event.Direction -> handleDirection(event)
        }
    }

    private fun handleButton(event: Event.Button) {
        val code = idToButtonCode(event.id) ?: return
        val pressed = event.action == MotionEvent.ACTION_DOWN
        AndroidHostInterface.getInstance().setControllerButtonState(mControllerIndex, code, pressed)
    }

    private fun handleDirection(event: Event.Direction) {
        when (event.id) {
            ID_DPAD -> {
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
            ID_LEFT_STICK -> {
                val xCode = AndroidHostInterface.getControllerAxisCode(mControllerType, "LeftX")
                val yCode = AndroidHostInterface.getControllerAxisCode(mControllerType, "LeftY")
                if (xCode >= 0) AndroidHostInterface.getInstance()
                    .setControllerAxisState(mControllerIndex, xCode, event.xAxis)
                if (yCode >= 0) AndroidHostInterface.getInstance()
                    .setControllerAxisState(mControllerIndex, yCode, event.yAxis)
            }
            ID_RIGHT_STICK -> {
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
            ID_CROSS     -> "Cross"
            ID_CIRCLE    -> "Circle"
            ID_SQUARE    -> "Square"
            ID_TRIANGLE  -> "Triangle"
            ID_L1        -> "L1"
            ID_L2        -> "L2"
            ID_R1        -> "R1"
            ID_R2        -> "R2"
            ID_SELECT    -> "Select"
            ID_START     -> "Start"
            ID_L3        -> "L3"
            ID_R3        -> "R3"
            else         -> return null
        }
        val code = AndroidHostInterface.getControllerButtonCode(mControllerType, name)
        return if (code >= 0) code else null
    }
}