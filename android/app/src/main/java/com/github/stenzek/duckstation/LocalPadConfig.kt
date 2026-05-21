package com.github.stenzek.duckstation

// ============================================================
// LOCAL PAD CONFIG - GITIGNORED, DO NOT COMMIT
// ============================================================
// Drop this file at:
//   android/app/src/main/java/com/github/stenzek/duckstation/LocalPadConfig.kt
//
// Controls the RadialGamePad layout for both left and right pads.
// Change socket counts, button labels, positions, or stick sizes freely.
// No other source files need to be touched.
// ============================================================

object LocalPadConfig {

    // Size of each pad widget (dp)
    const val PAD_SIZE = 25f

    // Buttons that toggle on tap instead of acting as momentary press
    val toggleButtons = setOf<Int>()

    // ---------------------------------------------------------------
    // Left pad — matches original hardcoded buildLeftPadConfig()
    // 12 sockets, D-pad center, Empty@2, SEL@2, L1@3, L2@4
    // ---------------------------------------------------------------
    val leftPad = PadConfig(
        socketCount = 12,
        primaryDial = PrimaryDialDef.Cross(id = PsxIds.DPAD, useDiagonals = false),
        secondaryDials = listOf(
            PadDial.Empty(
                index    = 2,
                spread   = 1,
                scale    = 0.5f,
                distance = 2f
            ),
            PadDial.Button(
                index    = 2,
                scale    = 1f,
                distance = 0f,
                id       = PsxIds.SELECT,
                label    = "SEL"
            ),
            PadDial.Button(
                index    = 3,
                scale    = 1f,
                distance = 0f,
                id       = PsxIds.L1,
                label    = "L1"
            ),
            PadDial.Button(
                index    = 4,
                scale    = 1f,
                distance = 0f,
                id       = PsxIds.L2,
                label    = "L2"
            ),
        )
    )

    // ---------------------------------------------------------------
    // Right pad — matches original hardcoded buildRightPadConfig()
    // 12 sockets, face buttons center, Empty@4, R1@2, R2@3, START@4
    // ---------------------------------------------------------------
    val rightPad = PadConfig(
        socketCount = 12,
        primaryDial = PrimaryDialDef.Buttons(
            dials = listOf(
                ButtonDef(id = PsxIds.CIRCLE,   label = "○"),
                ButtonDef(id = PsxIds.TRIANGLE, label = "△"),
                ButtonDef(id = PsxIds.SQUARE,   label = "□"),
                ButtonDef(id = PsxIds.CROSS,    label = "✕"),
            )
        ),
        secondaryDials = listOf(
            PadDial.Empty(
                index    = 4,
                spread   = 1,
                scale    = 0.5f,
                distance = 2f
            ),
            PadDial.Button(
                index    = 2,
                scale    = 1f,
                distance = 0f,
                id       = PsxIds.R1,
                label    = "R1"
            ),
            PadDial.Button(
                index    = 3,
                scale    = 1f,
                distance = 0f,
                id       = PsxIds.R2,
                label    = "R2"
            ),
            PadDial.Button(
                index    = 4,
                scale    = 1f,
                distance = 0f,
                id       = PsxIds.START,
                label    = "START"
            ),
        )
    )
}

// ---------------------------------------------------------------
// Data model — mirrors the shape RadialGamePadControllerView expects
// ---------------------------------------------------------------

data class PadConfig(
    val socketCount: Int,
    val primaryDial: PrimaryDialDef,
    val secondaryDials: List<PadDial>
)

data class ButtonDef(val id: Int, val label: String)

sealed class PrimaryDialDef {
    data class Cross(val id: Int, val useDiagonals: Boolean) : PrimaryDialDef()
    data class Buttons(val dials: List<ButtonDef>) : PrimaryDialDef()
}

sealed class PadDial {
    data class Empty(val index: Int, val spread: Int, val scale: Float, val distance: Float) : PadDial()
    data class Button(val index: Int, val scale: Float, val distance: Float, val id: Int, val label: String) : PadDial()
    data class Stick(val index: Int, val spread: Int, val scale: Float, val distance: Float, val id: Int) : PadDial()
}

// ---------------------------------------------------------------
// PS1 button ID constants — referenced by both pads and the view
// ---------------------------------------------------------------
object PsxIds {
    const val DPAD        = 0
    const val LEFT_STICK  = 1
    const val RIGHT_STICK = 2

    const val CROSS       = 10
    const val CIRCLE      = 11
    const val SQUARE      = 12
    const val TRIANGLE    = 13

    const val L1          = 20
    const val L2          = 21
    const val R1          = 22
    const val R2          = 23

    const val SELECT      = 30
    const val START       = 31
    const val L3          = 32
    const val R3          = 33
}