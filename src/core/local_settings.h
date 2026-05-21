// ============================================================
// LOCAL SETTINGS - GITIGNORED, DO NOT COMMIT
// ============================================================
// Drop this file at:
//   src/core/local_settings.h
//
// Overrides the default values baked into Settings::Load().
// Change values here freely — no other source files need to
// be touched. settings.cpp reads these via __has_include.
// ============================================================

// [Console]
#define LOCAL_REGION                            "Auto"
#define LOCAL_ENABLE_8MB_RAM                    false

// [Main]
#define LOCAL_EMULATION_SPEED                   1.0f
#define LOCAL_FAST_FORWARD_SPEED                0.0f
#define LOCAL_TURBO_SPEED                       0.0f
#define LOCAL_SYNC_TO_HOST_REFRESH_RATE         false
#define LOCAL_INCREASE_TIMER_RESOLUTION         true
#define LOCAL_START_PAUSED                      false
#define LOCAL_START_FULLSCREEN                  true
#define LOCAL_PAUSE_ON_FOCUS_LOSS               false
#define LOCAL_PAUSE_ON_MENU                     true
#define LOCAL_SAVE_STATE_ON_EXIT                true
#define LOCAL_CONFIRM_POWER_OFF                 false
#define LOCAL_LOAD_DEVICES_FROM_SAVE_STATES     true
#define LOCAL_APPLY_GAME_SETTINGS               true
#define LOCAL_AUTO_LOAD_CHEATS                  false
#define LOCAL_DISABLE_ALL_ENHANCEMENTS          false
#define LOCAL_REWIND_ENABLE                     false
#define LOCAL_REWIND_FREQUENCY                  10.0f
#define LOCAL_REWIND_SAVE_SLOTS                 10
#define LOCAL_RUNAHEAD_FRAME_COUNT              0

// [CPU]
#define LOCAL_CPU_EXECUTION_MODE                "Recompiler"
#define LOCAL_CPU_OVERCLOCK_ENABLE              false
#define LOCAL_CPU_OVERCLOCK_NUMERATOR           1
#define LOCAL_CPU_OVERCLOCK_DENOMINATOR         1
#define LOCAL_CPU_RECOMPILER_MEMORY_EXCEPTIONS  false
#define LOCAL_CPU_RECOMPILER_ICACHE             false
#define LOCAL_CPU_FASTMEM_MODE                  "MMap"

// [GPU]
#define LOCAL_GPU_RENDERER                      "Vulkan"
#define LOCAL_GPU_RESOLUTION_SCALE              4
#define LOCAL_GPU_MULTISAMPLES                  2
#define LOCAL_GPU_USE_THREAD                    true
#define LOCAL_GPU_THREADED_PRESENTATION         true
#define LOCAL_GPU_USE_SOFTWARE_READBACKS        false
#define LOCAL_GPU_TRUE_COLOR                    true
#define LOCAL_GPU_SCALED_DITHERING              false
#define LOCAL_GPU_TEXTURE_FILTER                "Nearest"
#define LOCAL_GPU_DOWNSAMPLE_MODE               "Disabled"
#define LOCAL_GPU_DISABLE_INTERLACING           true
#define LOCAL_GPU_FORCE_NTSC_TIMINGS            false
#define LOCAL_GPU_WIDESCREEN_HACK               true
#define LOCAL_GPU_24BIT_CHROMA_SMOOTHING        true
#define LOCAL_GPU_PGXP_ENABLE                   true
#define LOCAL_GPU_PGXP_CULLING                  true
#define LOCAL_GPU_PGXP_TEXTURE_CORRECTION       true
#define LOCAL_GPU_PGXP_VERTEX_CACHE             true
#define LOCAL_GPU_PGXP_CPU                      false
#define LOCAL_GPU_PGXP_PRESERVE_PROJ_FP         false
#define LOCAL_GPU_PGXP_TOLERANCE                -1.0f
#define LOCAL_GPU_PGXP_DEPTH_BUFFER             false
#define LOCAL_GPU_PGXP_DEPTH_THRESHOLD          300.0f

// [Display]
#define LOCAL_DISPLAY_CROP_MODE                 "Overscan"
#define LOCAL_DISPLAY_ASPECT_RATIO              "16:9"
#define LOCAL_DISPLAY_FORCE_4_3_FOR_24BIT       false
#define LOCAL_DISPLAY_LINEAR_FILTERING          true
#define LOCAL_DISPLAY_INTEGER_SCALING           false
#define LOCAL_DISPLAY_STRETCH                   false
#define LOCAL_DISPLAY_POST_PROCESSING           false
#define LOCAL_DISPLAY_SHOW_OSD_MESSAGES         false
#define LOCAL_DISPLAY_SHOW_FPS                  false
#define LOCAL_DISPLAY_SHOW_VPS                  false
#define LOCAL_DISPLAY_SHOW_SPEED                false
#define LOCAL_DISPLAY_SHOW_RESOLUTION           false
#define LOCAL_DISPLAY_ALL_FRAMES                false
#define LOCAL_DISPLAY_VSYNC                     true
#define LOCAL_DISPLAY_MAX_FPS                   60.0f

// [CDROM]
#define LOCAL_CDROM_READ_THREAD                 true
#define LOCAL_CDROM_REGION_CHECK                false
#define LOCAL_CDROM_LOAD_IMAGE_TO_RAM           false
#define LOCAL_CDROM_MUTE_CD_AUDIO               false
#define LOCAL_CDROM_READ_SPEEDUP                4
#define LOCAL_CDROM_SEEK_SPEEDUP                1

// [Audio]
#define LOCAL_AUDIO_BACKEND                     "OpenSLES"
#define LOCAL_AUDIO_OUTPUT_VOLUME               100
#define LOCAL_AUDIO_FAST_FORWARD_VOLUME         100
#define LOCAL_AUDIO_RESAMPLING                  false
#define LOCAL_AUDIO_OUTPUT_MUTED                false
#define LOCAL_AUDIO_SYNC                        false
#define LOCAL_AUDIO_DUMP_ON_BOOT                false

// [BIOS]
#define LOCAL_BIOS_PATCH_TTY_ENABLE             false
#define LOCAL_BIOS_PATCH_FAST_BOOT              true

// [Controllers]
#define LOCAL_CONTROLLER_1_TYPE                 "AnalogController"
#define LOCAL_CONTROLLER_2_TYPE                 "None"

// [MemoryCards]
#define LOCAL_MEMORY_CARD_1_TYPE                "PerGameTitle"
#define LOCAL_MEMORY_CARD_2_TYPE                "None"
#define LOCAL_MEMORY_CARD_USE_PLAYLIST_TITLE    true

// [ControllerPorts]
#define LOCAL_MULTITAP_MODE                     "Disabled"

// [Hacks]
#define LOCAL_DMA_MAX_SLICE_TICKS               1000
#define LOCAL_DMA_HALT_TICKS                    100
#define LOCAL_GPU_FIFO_SIZE                     16
#define LOCAL_GPU_MAX_RUN_AHEAD                 128

// [TextureReplacements]
#define LOCAL_TEXTURE_REPLACEMENTS_ENABLE   false
#define LOCAL_TEXTURE_REPLACEMENTS_PRELOAD  false

// [Logging]
#define LOCAL_LOG_LEVEL                         "Info"
#define LOCAL_LOG_TO_CONSOLE                    false
#define LOCAL_LOG_TO_DEBUG                      false
#define LOCAL_LOG_TO_WINDOW                     false
#define LOCAL_LOG_TO_FILE                       false