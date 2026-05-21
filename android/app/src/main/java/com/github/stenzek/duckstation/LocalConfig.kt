package com.github.stenzek.duckstation

// ============================================================
// LOCAL CONFIG - GITIGNORED, DO NOT COMMIT
// ============================================================
// Drop this file at:
//   android/app/src/main/java/com/github/stenzek/duckstation/LocalConfig.kt
//
// Game-specific values consumed at runtime.
// No other source files need to be touched.
// ============================================================

object LocalConfig {

    // ---------------------------------------------------------------
    // Asset file names
    // Must match the actual filenames inside your assets/ folder.
    // ---------------------------------------------------------------
    const val GAME_SERIAL    = "SCUS-94163"        // replace with your disc serial
    const val HAS_TEXTURES   = false               // set true to extract assets/textures.zip

    // BIOS
    const val BIOS_ASSET_NAME = "PSXONPSP660.bin"  // filename in assets/
    const val BIOS_FILE_NAME  = "PSXONPSP660.bin"  // filename written to bios dir

    // Game
    const val GAME_ASSET_NAME = "game.pbp"         // filename in assets/
    const val GAME_FILE_NAME  = "game.pbp"          // filename written to external storage
}