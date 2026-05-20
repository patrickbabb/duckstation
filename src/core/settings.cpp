#include "settings.h"
#include "common/assert.h"
#include "common/file_system.h"
#include "common/make_array.h"
#include "common/string_util.h"
#include "host_display.h"
#include "host_interface.h"
#include <algorithm>
#include <array>
#include <cctype>
#include <numeric>

#if __has_include("local_settings.h")
#include "local_settings.h"
#endif

// Fall back to upstream defaults if local_settings.h is absent
#ifndef LOCAL_REGION
#define LOCAL_REGION                            Settings::GetConsoleRegionName(Settings::DEFAULT_CONSOLE_REGION)
#endif
#ifndef LOCAL_ENABLE_8MB_RAM
#define LOCAL_ENABLE_8MB_RAM                    false
#endif
#ifndef LOCAL_EMULATION_SPEED
#define LOCAL_EMULATION_SPEED                   1.0f
#endif
#ifndef LOCAL_FAST_FORWARD_SPEED
#define LOCAL_FAST_FORWARD_SPEED                0.0f
#endif
#ifndef LOCAL_TURBO_SPEED
#define LOCAL_TURBO_SPEED                       0.0f
#endif
#ifndef LOCAL_SYNC_TO_HOST_REFRESH_RATE
#define LOCAL_SYNC_TO_HOST_REFRESH_RATE         false
#endif
#ifndef LOCAL_INCREASE_TIMER_RESOLUTION
#define LOCAL_INCREASE_TIMER_RESOLUTION         true
#endif
#ifndef LOCAL_START_PAUSED
#define LOCAL_START_PAUSED                      false
#endif
#ifndef LOCAL_START_FULLSCREEN
#define LOCAL_START_FULLSCREEN                  true
#endif
#ifndef LOCAL_PAUSE_ON_FOCUS_LOSS
#define LOCAL_PAUSE_ON_FOCUS_LOSS               false
#endif
#ifndef LOCAL_PAUSE_ON_MENU
#define LOCAL_PAUSE_ON_MENU                     true
#endif
#ifndef LOCAL_SAVE_STATE_ON_EXIT
#define LOCAL_SAVE_STATE_ON_EXIT                true
#endif
#ifndef LOCAL_CONFIRM_POWER_OFF
#define LOCAL_CONFIRM_POWER_OFF                 true
#endif
#ifndef LOCAL_LOAD_DEVICES_FROM_SAVE_STATES
#define LOCAL_LOAD_DEVICES_FROM_SAVE_STATES     true
#endif
#ifndef LOCAL_APPLY_GAME_SETTINGS
#define LOCAL_APPLY_GAME_SETTINGS               true
#endif
#ifndef LOCAL_AUTO_LOAD_CHEATS
#define LOCAL_AUTO_LOAD_CHEATS                  true
#endif
#ifndef LOCAL_DISABLE_ALL_ENHANCEMENTS
#define LOCAL_DISABLE_ALL_ENHANCEMENTS          false
#endif
#ifndef LOCAL_REWIND_ENABLE
#define LOCAL_REWIND_ENABLE                     false
#endif
#ifndef LOCAL_REWIND_FREQUENCY
#define LOCAL_REWIND_FREQUENCY                  10.0f
#endif
#ifndef LOCAL_REWIND_SAVE_SLOTS
#define LOCAL_REWIND_SAVE_SLOTS                 10
#endif
#ifndef LOCAL_RUNAHEAD_FRAME_COUNT
#define LOCAL_RUNAHEAD_FRAME_COUNT              0
#endif
#ifndef LOCAL_CPU_EXECUTION_MODE
#define LOCAL_CPU_EXECUTION_MODE                Settings::GetCPUExecutionModeName(Settings::DEFAULT_CPU_EXECUTION_MODE)
#endif
#ifndef LOCAL_CPU_OVERCLOCK_ENABLE
#define LOCAL_CPU_OVERCLOCK_ENABLE              false
#endif
#ifndef LOCAL_CPU_OVERCLOCK_NUMERATOR
#define LOCAL_CPU_OVERCLOCK_NUMERATOR           1
#endif
#ifndef LOCAL_CPU_OVERCLOCK_DENOMINATOR
#define LOCAL_CPU_OVERCLOCK_DENOMINATOR         1
#endif
#ifndef LOCAL_CPU_RECOMPILER_MEMORY_EXCEPTIONS
#define LOCAL_CPU_RECOMPILER_MEMORY_EXCEPTIONS  false
#endif
#ifndef LOCAL_CPU_RECOMPILER_ICACHE
#define LOCAL_CPU_RECOMPILER_ICACHE             false
#endif
#ifndef LOCAL_CPU_FASTMEM_MODE
#define LOCAL_CPU_FASTMEM_MODE                  Settings::GetCPUFastmemModeName(Settings::DEFAULT_CPU_FASTMEM_MODE)
#endif
#ifndef LOCAL_GPU_RENDERER
#define LOCAL_GPU_RENDERER                      Settings::GetRendererName(Settings::DEFAULT_GPU_RENDERER)
#endif
#ifndef LOCAL_GPU_RESOLUTION_SCALE
#define LOCAL_GPU_RESOLUTION_SCALE              8
#endif
#ifndef LOCAL_GPU_MULTISAMPLES
#define LOCAL_GPU_MULTISAMPLES                  1
#endif
#ifndef LOCAL_GPU_USE_THREAD
#define LOCAL_GPU_USE_THREAD                    true
#endif
#ifndef LOCAL_GPU_THREADED_PRESENTATION
#define LOCAL_GPU_THREADED_PRESENTATION         true
#endif
#ifndef LOCAL_GPU_USE_SOFTWARE_READBACKS
#define LOCAL_GPU_USE_SOFTWARE_READBACKS        false
#endif
#ifndef LOCAL_GPU_TRUE_COLOR
#define LOCAL_GPU_TRUE_COLOR                    true
#endif
#ifndef LOCAL_GPU_SCALED_DITHERING
#define LOCAL_GPU_SCALED_DITHERING              false
#endif
#ifndef LOCAL_GPU_TEXTURE_FILTER
#define LOCAL_GPU_TEXTURE_FILTER                Settings::GetTextureFilterName(Settings::DEFAULT_GPU_TEXTURE_FILTER)
#endif
#ifndef LOCAL_GPU_DOWNSAMPLE_MODE
#define LOCAL_GPU_DOWNSAMPLE_MODE               Settings::GetDownsampleModeName(Settings::DEFAULT_GPU_DOWNSAMPLE_MODE)
#endif
#ifndef LOCAL_GPU_DISABLE_INTERLACING
#define LOCAL_GPU_DISABLE_INTERLACING           true
#endif
#ifndef LOCAL_GPU_FORCE_NTSC_TIMINGS
#define LOCAL_GPU_FORCE_NTSC_TIMINGS            false
#endif
#ifndef LOCAL_GPU_WIDESCREEN_HACK
#define LOCAL_GPU_WIDESCREEN_HACK               false
#endif
#ifndef LOCAL_GPU_24BIT_CHROMA_SMOOTHING
#define LOCAL_GPU_24BIT_CHROMA_SMOOTHING        true
#endif
#ifndef LOCAL_GPU_PGXP_ENABLE
#define LOCAL_GPU_PGXP_ENABLE                   true
#endif
#ifndef LOCAL_GPU_PGXP_CULLING
#define LOCAL_GPU_PGXP_CULLING                  true
#endif
#ifndef LOCAL_GPU_PGXP_TEXTURE_CORRECTION
#define LOCAL_GPU_PGXP_TEXTURE_CORRECTION       true
#endif
#ifndef LOCAL_GPU_PGXP_VERTEX_CACHE
#define LOCAL_GPU_PGXP_VERTEX_CACHE             true
#endif
#ifndef LOCAL_GPU_PGXP_CPU
#define LOCAL_GPU_PGXP_CPU                      false
#endif
#ifndef LOCAL_GPU_PGXP_PRESERVE_PROJ_FP
#define LOCAL_GPU_PGXP_PRESERVE_PROJ_FP         false
#endif
#ifndef LOCAL_GPU_PGXP_TOLERANCE
#define LOCAL_GPU_PGXP_TOLERANCE                -1.0f
#endif
#ifndef LOCAL_GPU_PGXP_DEPTH_BUFFER
#define LOCAL_GPU_PGXP_DEPTH_BUFFER             false
#endif
#ifndef LOCAL_GPU_PGXP_DEPTH_THRESHOLD
#define LOCAL_GPU_PGXP_DEPTH_THRESHOLD          DEFAULT_GPU_PGXP_DEPTH_THRESHOLD
#endif
#ifndef LOCAL_DISPLAY_CROP_MODE
#define LOCAL_DISPLAY_CROP_MODE                 Settings::GetDisplayCropModeName(Settings::DEFAULT_DISPLAY_CROP_MODE)
#endif
#ifndef LOCAL_DISPLAY_ASPECT_RATIO
#define LOCAL_DISPLAY_ASPECT_RATIO              Settings::GetDisplayAspectRatioName(Settings::DEFAULT_DISPLAY_ASPECT_RATIO)
#endif
#ifndef LOCAL_DISPLAY_FORCE_4_3_FOR_24BIT
#define LOCAL_DISPLAY_FORCE_4_3_FOR_24BIT       false
#endif
#ifndef LOCAL_DISPLAY_LINEAR_FILTERING
#define LOCAL_DISPLAY_LINEAR_FILTERING          true
#endif
#ifndef LOCAL_DISPLAY_INTEGER_SCALING
#define LOCAL_DISPLAY_INTEGER_SCALING           false
#endif
#ifndef LOCAL_DISPLAY_STRETCH
#define LOCAL_DISPLAY_STRETCH                   false
#endif
#ifndef LOCAL_DISPLAY_POST_PROCESSING
#define LOCAL_DISPLAY_POST_PROCESSING           true
#endif
#ifndef LOCAL_DISPLAY_SHOW_OSD_MESSAGES
#define LOCAL_DISPLAY_SHOW_OSD_MESSAGES         false
#endif
#ifndef LOCAL_DISPLAY_SHOW_FPS
#define LOCAL_DISPLAY_SHOW_FPS                  false
#endif
#ifndef LOCAL_DISPLAY_SHOW_VPS
#define LOCAL_DISPLAY_SHOW_VPS                  false
#endif
#ifndef LOCAL_DISPLAY_SHOW_SPEED
#define LOCAL_DISPLAY_SHOW_SPEED                false
#endif
#ifndef LOCAL_DISPLAY_SHOW_RESOLUTION
#define LOCAL_DISPLAY_SHOW_RESOLUTION           false
#endif
#ifndef LOCAL_DISPLAY_ALL_FRAMES
#define LOCAL_DISPLAY_ALL_FRAMES                false
#endif
#ifndef LOCAL_DISPLAY_VSYNC
#define LOCAL_DISPLAY_VSYNC                     DEFAULT_VSYNC_VALUE
#endif
#ifndef LOCAL_DISPLAY_MAX_FPS
#define LOCAL_DISPLAY_MAX_FPS                   DEFAULT_DISPLAY_MAX_FPS
#endif
#ifndef LOCAL_CDROM_READ_THREAD
#define LOCAL_CDROM_READ_THREAD                 true
#endif
#ifndef LOCAL_CDROM_REGION_CHECK
#define LOCAL_CDROM_REGION_CHECK                false
#endif
#ifndef LOCAL_CDROM_LOAD_IMAGE_TO_RAM
#define LOCAL_CDROM_LOAD_IMAGE_TO_RAM           false
#endif
#ifndef LOCAL_CDROM_MUTE_CD_AUDIO
#define LOCAL_CDROM_MUTE_CD_AUDIO               false
#endif
#ifndef LOCAL_CDROM_READ_SPEEDUP
#define LOCAL_CDROM_READ_SPEEDUP                1
#endif
#ifndef LOCAL_CDROM_SEEK_SPEEDUP
#define LOCAL_CDROM_SEEK_SPEEDUP                1
#endif
#ifndef LOCAL_AUDIO_BACKEND
#define LOCAL_AUDIO_BACKEND                     Settings::GetAudioBackendName(Settings::DEFAULT_AUDIO_BACKEND)
#endif
#ifndef LOCAL_AUDIO_OUTPUT_VOLUME
#define LOCAL_AUDIO_OUTPUT_VOLUME               100
#endif
#ifndef LOCAL_AUDIO_FAST_FORWARD_VOLUME
#define LOCAL_AUDIO_FAST_FORWARD_VOLUME         100
#endif
#ifndef LOCAL_AUDIO_RESAMPLING
#define LOCAL_AUDIO_RESAMPLING                  true
#endif
#ifndef LOCAL_AUDIO_OUTPUT_MUTED
#define LOCAL_AUDIO_OUTPUT_MUTED                false
#endif
#ifndef LOCAL_AUDIO_SYNC
#define LOCAL_AUDIO_SYNC                        true
#endif
#ifndef LOCAL_AUDIO_DUMP_ON_BOOT
#define LOCAL_AUDIO_DUMP_ON_BOOT                false
#endif
#ifndef LOCAL_BIOS_PATCH_TTY_ENABLE
#define LOCAL_BIOS_PATCH_TTY_ENABLE             false
#endif
#ifndef LOCAL_BIOS_PATCH_FAST_BOOT
#define LOCAL_BIOS_PATCH_FAST_BOOT              DEFAULT_FAST_BOOT_VALUE
#endif
#ifndef LOCAL_CONTROLLER_1_TYPE
#define LOCAL_CONTROLLER_1_TYPE                 Settings::GetControllerTypeName(Settings::DEFAULT_CONTROLLER_1_TYPE)
#endif
#ifndef LOCAL_CONTROLLER_2_TYPE
#define LOCAL_CONTROLLER_2_TYPE                 Settings::GetControllerTypeName(Settings::DEFAULT_CONTROLLER_2_TYPE)
#endif
#ifndef LOCAL_MEMORY_CARD_1_TYPE
#define LOCAL_MEMORY_CARD_1_TYPE                Settings::GetMemoryCardTypeName(Settings::DEFAULT_MEMORY_CARD_1_TYPE)
#endif
#ifndef LOCAL_MEMORY_CARD_2_TYPE
#define LOCAL_MEMORY_CARD_2_TYPE                Settings::GetMemoryCardTypeName(Settings::DEFAULT_MEMORY_CARD_2_TYPE)
#endif
#ifndef LOCAL_MEMORY_CARD_USE_PLAYLIST_TITLE
#define LOCAL_MEMORY_CARD_USE_PLAYLIST_TITLE    true
#endif
#ifndef LOCAL_MULTITAP_MODE
#define LOCAL_MULTITAP_MODE                     Settings::GetMultitapModeName(Settings::DEFAULT_MULTITAP_MODE)
#endif
#ifndef LOCAL_DMA_MAX_SLICE_TICKS
#define LOCAL_DMA_MAX_SLICE_TICKS               DEFAULT_DMA_MAX_SLICE_TICKS
#endif
#ifndef LOCAL_DMA_HALT_TICKS
#define LOCAL_DMA_HALT_TICKS                    DEFAULT_DMA_HALT_TICKS
#endif
#ifndef LOCAL_GPU_FIFO_SIZE
#define LOCAL_GPU_FIFO_SIZE                     DEFAULT_GPU_FIFO_SIZE
#endif
#ifndef LOCAL_GPU_MAX_RUN_AHEAD
#define LOCAL_GPU_MAX_RUN_AHEAD                 DEFAULT_GPU_MAX_RUN_AHEAD
#endif
#ifndef LOCAL_LOG_LEVEL
#define LOCAL_LOG_LEVEL                         Settings::GetLogLevelName(Settings::DEFAULT_LOG_LEVEL)
#endif
#ifndef LOCAL_LOG_TO_CONSOLE
#define LOCAL_LOG_TO_CONSOLE                    DEFAULT_LOG_TO_CONSOLE
#endif
#ifndef LOCAL_LOG_TO_DEBUG
#define LOCAL_LOG_TO_DEBUG                      false
#endif
#ifndef LOCAL_LOG_TO_WINDOW
#define LOCAL_LOG_TO_WINDOW                     false
#endif
#ifndef LOCAL_LOG_TO_FILE
#define LOCAL_LOG_TO_FILE                       false
#endif
#ifndef LOCAL_TEXTURE_REPLACEMENTS_ENABLE
#define LOCAL_TEXTURE_REPLACEMENTS_ENABLE       false
#endif
#ifndef LOCAL_TEXTURE_REPLACEMENTS_PRELOAD
#define LOCAL_TEXTURE_REPLACEMENTS_PRELOAD      false
#endif

Settings g_settings;

SettingsInterface::~SettingsInterface() = default;

const char* SettingInfo::StringDefaultValue() const
{
    return default_value ? default_value : "";
}

bool SettingInfo::BooleanDefaultValue() const
{
    return default_value ? StringUtil::FromChars<bool>(default_value).value_or(false) : false;
}

s32 SettingInfo::IntegerDefaultValue() const
{
    return default_value ? StringUtil::FromChars<s32>(default_value).value_or(0) : 0;
}

s32 SettingInfo::IntegerMinValue() const
{
    static constexpr s32 fallback_value = std::numeric_limits<s32>::min();
    return min_value ? StringUtil::FromChars<s32>(min_value).value_or(fallback_value) : fallback_value;
}

s32 SettingInfo::IntegerMaxValue() const
{
    static constexpr s32 fallback_value = std::numeric_limits<s32>::max();
    return max_value ? StringUtil::FromChars<s32>(max_value).value_or(fallback_value) : fallback_value;
}

s32 SettingInfo::IntegerStepValue() const
{
    static constexpr s32 fallback_value = 1;
    return step_value ? StringUtil::FromChars<s32>(step_value).value_or(fallback_value) : fallback_value;
}

float SettingInfo::FloatDefaultValue() const
{
    return default_value ? StringUtil::FromChars<float>(default_value).value_or(0.0f) : 0.0f;
}

float SettingInfo::FloatMinValue() const
{
    static constexpr float fallback_value = std::numeric_limits<float>::min();
    return min_value ? StringUtil::FromChars<float>(min_value).value_or(fallback_value) : fallback_value;
}

float SettingInfo::FloatMaxValue() const
{
    static constexpr float fallback_value = std::numeric_limits<float>::max();
    return max_value ? StringUtil::FromChars<float>(max_value).value_or(fallback_value) : fallback_value;
}

float SettingInfo::FloatStepValue() const
{
    static constexpr float fallback_value = 0.1f;
    return step_value ? StringUtil::FromChars<float>(step_value).value_or(fallback_value) : fallback_value;
}

Settings::Settings() = default;

bool Settings::HasAnyPerGameMemoryCards() const
{
    return std::any_of(memory_card_types.begin(), memory_card_types.end(), [](MemoryCardType t) {
        return (t == MemoryCardType::PerGame || t == MemoryCardType::PerGameTitle);
    });
}

std::array<TinyString, NUM_CONTROLLER_AND_CARD_PORTS> Settings::GeneratePortLabels() const
{
    static constexpr std::array<std::array<bool, NUM_MULTITAPS>, static_cast<size_t>(MultitapMode::Count)>
            multitap_enabled_on_port = {{{false, false}, {true, false}, {false, true}, {true, true}}};

    std::array<TinyString, NUM_CONTROLLER_AND_CARD_PORTS> labels;

    u32 logical_port = 0;
    for (u32 physical_port = 0; physical_port < NUM_MULTITAPS; physical_port++)
    {
        if (multitap_enabled_on_port[static_cast<size_t>(multitap_mode)][physical_port])
        {
            for (u32 i = 0; i < 4; i++)
            {
                labels[logical_port] = TinyString::FromFormat("Port %u%c", physical_port + 1u, 'A' + i);
                logical_port++;
            }
        }
        else
        {
            labels[logical_port] = TinyString::FromFormat("Port %u", physical_port + 1u);
            logical_port++;
        }
    }

    return labels;
}

void Settings::CPUOverclockPercentToFraction(u32 percent, u32* numerator, u32* denominator)
{
    const u32 percent_gcd = std::gcd(percent, 100);
    *numerator = percent / percent_gcd;
    *denominator = 100u / percent_gcd;
}

u32 Settings::CPUOverclockFractionToPercent(u32 numerator, u32 denominator)
{
    return (numerator * 100u) / denominator;
}

void Settings::SetCPUOverclockPercent(u32 percent)
{
    CPUOverclockPercentToFraction(percent, &cpu_overclock_numerator, &cpu_overclock_denominator);
}

u32 Settings::GetCPUOverclockPercent() const
{
    return CPUOverclockFractionToPercent(cpu_overclock_numerator, cpu_overclock_denominator);
}

void Settings::UpdateOverclockActive()
{
    cpu_overclock_active = (cpu_overclock_enable && (cpu_overclock_numerator != 1 || cpu_overclock_denominator != 1));
}

void Settings::Load(SettingsInterface& si)
{
    region =
            ParseConsoleRegionName(si.GetStringValue("Console", "Region", LOCAL_REGION).c_str())
                    .value_or(DEFAULT_CONSOLE_REGION);
    enable_8mb_ram = si.GetBoolValue("Console", "Enable8MBRAM", LOCAL_ENABLE_8MB_RAM);

    emulation_speed = si.GetFloatValue("Main", "EmulationSpeed", LOCAL_EMULATION_SPEED);
    fast_forward_speed = si.GetFloatValue("Main", "FastForwardSpeed", LOCAL_FAST_FORWARD_SPEED);
    turbo_speed = si.GetFloatValue("Main", "TurboSpeed", LOCAL_TURBO_SPEED);
    sync_to_host_refresh_rate = si.GetBoolValue("Main", "SyncToHostRefreshRate", LOCAL_SYNC_TO_HOST_REFRESH_RATE);
    increase_timer_resolution = si.GetBoolValue("Main", "IncreaseTimerResolution", LOCAL_INCREASE_TIMER_RESOLUTION);
    start_paused = si.GetBoolValue("Main", "StartPaused", LOCAL_START_PAUSED);
    start_fullscreen = si.GetBoolValue("Main", "StartFullscreen", LOCAL_START_FULLSCREEN);
    pause_on_focus_loss = si.GetBoolValue("Main", "PauseOnFocusLoss", LOCAL_PAUSE_ON_FOCUS_LOSS);
    pause_on_menu = si.GetBoolValue("Main", "PauseOnMenu", LOCAL_PAUSE_ON_MENU);
    save_state_on_exit = si.GetBoolValue("Main", "SaveStateOnExit", LOCAL_SAVE_STATE_ON_EXIT);
    confim_power_off = si.GetBoolValue("Main", "ConfirmPowerOff", LOCAL_CONFIRM_POWER_OFF);
    load_devices_from_save_states = si.GetBoolValue("Main", "LoadDevicesFromSaveStates", LOCAL_LOAD_DEVICES_FROM_SAVE_STATES);
    apply_game_settings = si.GetBoolValue("Main", "ApplyGameSettings", LOCAL_APPLY_GAME_SETTINGS);
    auto_load_cheats = si.GetBoolValue("Main", "AutoLoadCheats", LOCAL_AUTO_LOAD_CHEATS);
    disable_all_enhancements = si.GetBoolValue("Main", "DisableAllEnhancements", LOCAL_DISABLE_ALL_ENHANCEMENTS);
    rewind_enable = si.GetBoolValue("Main", "RewindEnable", LOCAL_REWIND_ENABLE);
    rewind_save_frequency = si.GetFloatValue("Main", "RewindFrequency", LOCAL_REWIND_FREQUENCY);
    rewind_save_slots = static_cast<u32>(si.GetIntValue("Main", "RewindSaveSlots", LOCAL_REWIND_SAVE_SLOTS));
    runahead_frames = static_cast<u32>(si.GetIntValue("Main", "RunaheadFrameCount", LOCAL_RUNAHEAD_FRAME_COUNT));

    cpu_execution_mode =
            ParseCPUExecutionMode(si.GetStringValue("CPU", "ExecutionMode", LOCAL_CPU_EXECUTION_MODE).c_str())
                    .value_or(DEFAULT_CPU_EXECUTION_MODE);
    cpu_overclock_numerator = std::max(si.GetIntValue("CPU", "OverclockNumerator", LOCAL_CPU_OVERCLOCK_NUMERATOR), 1);
    cpu_overclock_denominator = std::max(si.GetIntValue("CPU", "OverclockDenominator", LOCAL_CPU_OVERCLOCK_DENOMINATOR), 1);
    cpu_overclock_enable = si.GetBoolValue("CPU", "OverclockEnable", LOCAL_CPU_OVERCLOCK_ENABLE);
    UpdateOverclockActive();
    cpu_recompiler_memory_exceptions = si.GetBoolValue("CPU", "RecompilerMemoryExceptions", LOCAL_CPU_RECOMPILER_MEMORY_EXCEPTIONS);
    cpu_recompiler_icache = si.GetBoolValue("CPU", "RecompilerICache", LOCAL_CPU_RECOMPILER_ICACHE);
    cpu_fastmem_mode =
            ParseCPUFastmemMode(si.GetStringValue("CPU", "FastmemMode", LOCAL_CPU_FASTMEM_MODE).c_str())
                    .value_or(DEFAULT_CPU_FASTMEM_MODE);

    gpu_renderer =
            ParseRendererName(si.GetStringValue("GPU", "Renderer", LOCAL_GPU_RENDERER).c_str())
                    .value_or(DEFAULT_GPU_RENDERER);
    gpu_adapter = si.GetStringValue("GPU", "Adapter", "");
    gpu_resolution_scale = static_cast<u32>(si.GetIntValue("GPU", "ResolutionScale", LOCAL_GPU_RESOLUTION_SCALE));
    gpu_multisamples = static_cast<u32>(si.GetIntValue("GPU", "Multisamples", LOCAL_GPU_MULTISAMPLES));
    gpu_use_debug_device = si.GetBoolValue("GPU", "UseDebugDevice", false);
    gpu_per_sample_shading = si.GetBoolValue("GPU", "PerSampleShading", false);
    gpu_use_thread = si.GetBoolValue("GPU", "UseThread", LOCAL_GPU_USE_THREAD);
    gpu_use_software_renderer_for_readbacks = si.GetBoolValue("GPU", "UseSoftwareRendererForReadbacks", LOCAL_GPU_USE_SOFTWARE_READBACKS);
    gpu_threaded_presentation = si.GetBoolValue("GPU", "ThreadedPresentation", LOCAL_GPU_THREADED_PRESENTATION);
    gpu_true_color = si.GetBoolValue("GPU", "TrueColor", LOCAL_GPU_TRUE_COLOR);
    gpu_scaled_dithering = si.GetBoolValue("GPU", "ScaledDithering", LOCAL_GPU_SCALED_DITHERING);
    gpu_texture_filter =
            ParseTextureFilterName(si.GetStringValue("GPU", "TextureFilter", LOCAL_GPU_TEXTURE_FILTER).c_str())
                    .value_or(DEFAULT_GPU_TEXTURE_FILTER);
    gpu_downsample_mode =
            ParseDownsampleModeName(si.GetStringValue("GPU", "DownsampleMode", LOCAL_GPU_DOWNSAMPLE_MODE).c_str())
                    .value_or(DEFAULT_GPU_DOWNSAMPLE_MODE);
    gpu_disable_interlacing = si.GetBoolValue("GPU", "DisableInterlacing", LOCAL_GPU_DISABLE_INTERLACING);
    gpu_force_ntsc_timings = si.GetBoolValue("GPU", "ForceNTSCTimings", LOCAL_GPU_FORCE_NTSC_TIMINGS);
    gpu_widescreen_hack = si.GetBoolValue("GPU", "WidescreenHack", LOCAL_GPU_WIDESCREEN_HACK);
    gpu_24bit_chroma_smoothing = si.GetBoolValue("GPU", "ChromaSmoothing24Bit", LOCAL_GPU_24BIT_CHROMA_SMOOTHING);
    gpu_pgxp_enable = si.GetBoolValue("GPU", "PGXPEnable", LOCAL_GPU_PGXP_ENABLE);
    gpu_pgxp_culling = si.GetBoolValue("GPU", "PGXPCulling", LOCAL_GPU_PGXP_CULLING);
    gpu_pgxp_texture_correction = si.GetBoolValue("GPU", "PGXPTextureCorrection", LOCAL_GPU_PGXP_TEXTURE_CORRECTION);
    gpu_pgxp_vertex_cache = si.GetBoolValue("GPU", "PGXPVertexCache", LOCAL_GPU_PGXP_VERTEX_CACHE);
    gpu_pgxp_cpu = si.GetBoolValue("GPU", "PGXPCPU", LOCAL_GPU_PGXP_CPU);
    gpu_pgxp_preserve_proj_fp = si.GetBoolValue("GPU", "PGXPPreserveProjFP", LOCAL_GPU_PGXP_PRESERVE_PROJ_FP);
    gpu_pgxp_tolerance = si.GetFloatValue("GPU", "PGXPTolerance", LOCAL_GPU_PGXP_TOLERANCE);
    gpu_pgxp_depth_buffer = si.GetBoolValue("GPU", "PGXPDepthBuffer", LOCAL_GPU_PGXP_DEPTH_BUFFER);
    SetPGXPDepthClearThreshold(si.GetFloatValue("GPU", "PGXPDepthClearThreshold", LOCAL_GPU_PGXP_DEPTH_THRESHOLD));

    display_crop_mode =
            ParseDisplayCropMode(si.GetStringValue("Display", "CropMode", LOCAL_DISPLAY_CROP_MODE).c_str())
                    .value_or(DEFAULT_DISPLAY_CROP_MODE);
    display_aspect_ratio =
            ParseDisplayAspectRatio(si.GetStringValue("Display", "AspectRatio", LOCAL_DISPLAY_ASPECT_RATIO).c_str())
                    .value_or(DEFAULT_DISPLAY_ASPECT_RATIO);
    display_aspect_ratio_custom_numerator = static_cast<u16>(
            std::clamp<int>(si.GetIntValue("Display", "CustomAspectRatioNumerator", 4), 1, std::numeric_limits<u16>::max()));
    display_aspect_ratio_custom_denominator = static_cast<u16>(
            std::clamp<int>(si.GetIntValue("Display", "CustomAspectRatioDenominator", 3), 1, std::numeric_limits<u16>::max()));
    display_force_4_3_for_24bit = si.GetBoolValue("Display", "Force4_3For24Bit", LOCAL_DISPLAY_FORCE_4_3_FOR_24BIT);
    display_active_start_offset = static_cast<s16>(si.GetIntValue("Display", "ActiveStartOffset", 0));
    display_active_end_offset = static_cast<s16>(si.GetIntValue("Display", "ActiveEndOffset", 0));
    display_line_start_offset = static_cast<s8>(si.GetIntValue("Display", "LineStartOffset", 0));
    display_line_end_offset = static_cast<s8>(si.GetIntValue("Display", "LineEndOffset", 0));
    display_linear_filtering = si.GetBoolValue("Display", "LinearFiltering", LOCAL_DISPLAY_LINEAR_FILTERING);
    display_integer_scaling = si.GetBoolValue("Display", "IntegerScaling", LOCAL_DISPLAY_INTEGER_SCALING);
    display_stretch = si.GetBoolValue("Display", "Stretch", LOCAL_DISPLAY_STRETCH);
    display_post_processing = si.GetBoolValue("Display", "PostProcessing", LOCAL_DISPLAY_POST_PROCESSING);
    display_show_osd_messages = si.GetBoolValue("Display", "ShowOSDMessages", LOCAL_DISPLAY_SHOW_OSD_MESSAGES);
    display_show_fps = si.GetBoolValue("Display", "ShowFPS", LOCAL_DISPLAY_SHOW_FPS);
    display_show_vps = si.GetBoolValue("Display", "ShowVPS", LOCAL_DISPLAY_SHOW_VPS);
    display_show_speed = si.GetBoolValue("Display", "ShowSpeed", LOCAL_DISPLAY_SHOW_SPEED);
    display_show_resolution = si.GetBoolValue("Display", "ShowResolution", LOCAL_DISPLAY_SHOW_RESOLUTION);
    display_all_frames = si.GetBoolValue("Display", "DisplayAllFrames", LOCAL_DISPLAY_ALL_FRAMES);
    video_sync_enabled = si.GetBoolValue("Display", "VSync", LOCAL_DISPLAY_VSYNC);
    display_post_process_chain = si.GetStringValue("Display", "PostProcessChain", "");
    display_max_fps = si.GetFloatValue("Display", "MaxFPS", LOCAL_DISPLAY_MAX_FPS);

    cdrom_read_thread = si.GetBoolValue("CDROM", "ReadThread", LOCAL_CDROM_READ_THREAD);
    cdrom_region_check = si.GetBoolValue("CDROM", "RegionCheck", LOCAL_CDROM_REGION_CHECK);
    cdrom_load_image_to_ram = si.GetBoolValue("CDROM", "LoadImageToRAM", LOCAL_CDROM_LOAD_IMAGE_TO_RAM);
    cdrom_mute_cd_audio = si.GetBoolValue("CDROM", "MuteCDAudio", LOCAL_CDROM_MUTE_CD_AUDIO);
    cdrom_read_speedup = si.GetIntValue("CDROM", "ReadSpeedup", LOCAL_CDROM_READ_SPEEDUP);
    cdrom_seek_speedup = si.GetIntValue("CDROM", "SeekSpeedup", LOCAL_CDROM_SEEK_SPEEDUP);

    audio_backend =
            ParseAudioBackend(si.GetStringValue("Audio", "Backend", LOCAL_AUDIO_BACKEND).c_str())
                    .value_or(DEFAULT_AUDIO_BACKEND);
    audio_output_volume = si.GetIntValue("Audio", "OutputVolume", LOCAL_AUDIO_OUTPUT_VOLUME);
    audio_fast_forward_volume = si.GetIntValue("Audio", "FastForwardVolume", LOCAL_AUDIO_FAST_FORWARD_VOLUME);
    audio_buffer_size = si.GetIntValue("Audio", "BufferSize", HostInterface::DEFAULT_AUDIO_BUFFER_SIZE);
    audio_resampling = si.GetBoolValue("Audio", "Resampling", LOCAL_AUDIO_RESAMPLING);
    audio_output_muted = si.GetBoolValue("Audio", "OutputMuted", LOCAL_AUDIO_OUTPUT_MUTED);
    audio_sync_enabled = si.GetBoolValue("Audio", "Sync", LOCAL_AUDIO_SYNC);
    audio_dump_on_boot = si.GetBoolValue("Audio", "DumpOnBoot", LOCAL_AUDIO_DUMP_ON_BOOT);

    dma_max_slice_ticks = si.GetIntValue("Hacks", "DMAMaxSliceTicks", LOCAL_DMA_MAX_SLICE_TICKS);
    dma_halt_ticks = si.GetIntValue("Hacks", "DMAHaltTicks", LOCAL_DMA_HALT_TICKS);
    gpu_fifo_size = static_cast<u32>(si.GetIntValue("Hacks", "GPUFIFOSize", LOCAL_GPU_FIFO_SIZE));
    gpu_max_run_ahead = si.GetIntValue("Hacks", "GPUMaxRunAhead", LOCAL_GPU_MAX_RUN_AHEAD);

    bios_patch_tty_enable = si.GetBoolValue("BIOS", "PatchTTYEnable", LOCAL_BIOS_PATCH_TTY_ENABLE);
    bios_patch_fast_boot = si.GetBoolValue("BIOS", "PatchFastBoot", LOCAL_BIOS_PATCH_FAST_BOOT);

    controller_types[0] =
            ParseControllerTypeName(si.GetStringValue("Controller1", "Type", LOCAL_CONTROLLER_1_TYPE).c_str())
                    .value_or(DEFAULT_CONTROLLER_1_TYPE);
    for (u32 i = 1; i < NUM_CONTROLLER_AND_CARD_PORTS; i++)
    {
        controller_types[i] =
                ParseControllerTypeName(si.GetStringValue(TinyString::FromFormat("Controller%u", i + 1u), "Type",
                                                          LOCAL_CONTROLLER_2_TYPE).c_str())
                        .value_or(DEFAULT_CONTROLLER_2_TYPE);
    }

    memory_card_types[0] =
            ParseMemoryCardTypeName(si.GetStringValue("MemoryCards", "Card1Type", LOCAL_MEMORY_CARD_1_TYPE).c_str())
                    .value_or(DEFAULT_MEMORY_CARD_1_TYPE);
    memory_card_types[1] =
            ParseMemoryCardTypeName(si.GetStringValue("MemoryCards", "Card2Type", LOCAL_MEMORY_CARD_2_TYPE).c_str())
                    .value_or(DEFAULT_MEMORY_CARD_2_TYPE);
    memory_card_paths[0] = si.GetStringValue("MemoryCards", "Card1Path", "");
    memory_card_paths[1] = si.GetStringValue("MemoryCards", "Card2Path", "");
    memory_card_directory = si.GetStringValue("MemoryCards", "Directory", "");
    memory_card_use_playlist_title = si.GetBoolValue("MemoryCards", "UsePlaylistTitle", LOCAL_MEMORY_CARD_USE_PLAYLIST_TITLE);

    multitap_mode =
            ParseMultitapModeName(si.GetStringValue("ControllerPorts", "MultitapMode", LOCAL_MULTITAP_MODE).c_str())
                    .value_or(DEFAULT_MULTITAP_MODE);

    log_level =
            ParseLogLevelName(si.GetStringValue("Logging", "LogLevel", LOCAL_LOG_LEVEL).c_str())
                    .value_or(DEFAULT_LOG_LEVEL);
    log_filter = si.GetStringValue("Logging", "LogFilter", "");
    log_to_console = si.GetBoolValue("Logging", "LogToConsole", LOCAL_LOG_TO_CONSOLE);
    log_to_debug = si.GetBoolValue("Logging", "LogToDebug", LOCAL_LOG_TO_DEBUG);
    log_to_window = si.GetBoolValue("Logging", "LogToWindow", LOCAL_LOG_TO_WINDOW);
    log_to_file = si.GetBoolValue("Logging", "LogToFile", LOCAL_LOG_TO_FILE);

    debugging.show_vram = si.GetBoolValue("Debug", "ShowVRAM");
    debugging.dump_cpu_to_vram_copies = si.GetBoolValue("Debug", "DumpCPUToVRAMCopies");
    debugging.dump_vram_to_cpu_copies = si.GetBoolValue("Debug", "DumpVRAMToCPUCopies");
    debugging.enable_gdb_server = si.GetBoolValue("Debug", "EnableGDBServer");
    debugging.gdb_server_port = static_cast<u16>(si.GetIntValue("Debug", "GDBServerPort"));
    debugging.show_gpu_state = si.GetBoolValue("Debug", "ShowGPUState");
    debugging.show_cdrom_state = si.GetBoolValue("Debug", "ShowCDROMState");
    debugging.show_spu_state = si.GetBoolValue("Debug", "ShowSPUState");
    debugging.show_timers_state = si.GetBoolValue("Debug", "ShowTimersState");
    debugging.show_mdec_state = si.GetBoolValue("Debug", "ShowMDECState");
    debugging.show_dma_state = si.GetBoolValue("Debug", "ShowDMAState");

    texture_replacements.enable_vram_write_replacements =
            si.GetBoolValue("TextureReplacements", "EnableVRAMWriteReplacements", false);
    texture_replacements.preload_textures = si.GetBoolValue("TextureReplacements", "PreloadTextures", false);
    texture_replacements.dump_vram_writes = si.GetBoolValue("TextureReplacements", "DumpVRAMWrites", false);
    texture_replacements.dump_vram_write_force_alpha_channel =
            si.GetBoolValue("TextureReplacements", "DumpVRAMWriteForceAlphaChannel", true);
    texture_replacements.dump_vram_write_width_threshold =
            si.GetIntValue("TextureReplacements", "DumpVRAMWriteWidthThreshold", 128);
    texture_replacements.dump_vram_write_height_threshold =
            si.GetIntValue("TextureReplacements", "DumpVRAMWriteHeightThreshold", 128);
}

void Settings::Save(SettingsInterface& si) const
{
    si.SetStringValue("Console", "Region", GetConsoleRegionName(region));
    si.SetBoolValue("Console", "Enable8MBRAM", enable_8mb_ram);

    si.SetFloatValue("Main", "EmulationSpeed", emulation_speed);
    si.SetFloatValue("Main", "FastForwardSpeed", fast_forward_speed);
    si.SetFloatValue("Main", "TurboSpeed", turbo_speed);
    si.SetBoolValue("Main", "SyncToHostRefreshRate", sync_to_host_refresh_rate);
    si.SetBoolValue("Main", "IncreaseTimerResolution", increase_timer_resolution);
    si.SetBoolValue("Main", "StartPaused", start_paused);
    si.SetBoolValue("Main", "StartFullscreen", start_fullscreen);
    si.SetBoolValue("Main", "PauseOnFocusLoss", pause_on_focus_loss);
    si.SetBoolValue("Main", "PauseOnMenu", pause_on_menu);
    si.SetBoolValue("Main", "SaveStateOnExit", save_state_on_exit);
    si.SetBoolValue("Main", "ConfirmPowerOff", confim_power_off);
    si.SetBoolValue("Main", "LoadDevicesFromSaveStates", load_devices_from_save_states);
    si.SetBoolValue("Main", "ApplyGameSettings", apply_game_settings);
    si.SetBoolValue("Main", "AutoLoadCheats", auto_load_cheats);
    si.SetBoolValue("Main", "DisableAllEnhancements", disable_all_enhancements);
    si.SetBoolValue("Main", "RewindEnable", rewind_enable);
    si.SetFloatValue("Main", "RewindFrequency", rewind_save_frequency);
    si.SetIntValue("Main", "RewindSaveSlots", rewind_save_slots);
    si.SetIntValue("Main", "RunaheadFrameCount", runahead_frames);

    si.SetStringValue("CPU", "ExecutionMode", GetCPUExecutionModeName(cpu_execution_mode));
    si.SetBoolValue("CPU", "OverclockEnable", cpu_overclock_enable);
    si.SetIntValue("CPU", "OverclockNumerator", cpu_overclock_numerator);
    si.SetIntValue("CPU", "OverclockDenominator", cpu_overclock_denominator);
    si.SetBoolValue("CPU", "RecompilerMemoryExceptions", cpu_recompiler_memory_exceptions);
    si.SetBoolValue("CPU", "RecompilerICache", cpu_recompiler_icache);
    si.SetStringValue("CPU", "FastmemMode", GetCPUFastmemModeName(cpu_fastmem_mode));

    si.SetStringValue("GPU", "Renderer", GetRendererName(gpu_renderer));
    si.SetStringValue("GPU", "Adapter", gpu_adapter.c_str());
    si.SetIntValue("GPU", "ResolutionScale", static_cast<long>(gpu_resolution_scale));
    si.SetIntValue("GPU", "Multisamples", static_cast<long>(gpu_multisamples));
    si.SetBoolValue("GPU", "UseDebugDevice", gpu_use_debug_device);
    si.SetBoolValue("GPU", "PerSampleShading", gpu_per_sample_shading);
    si.SetBoolValue("GPU", "UseThread", gpu_use_thread);
    si.SetBoolValue("GPU", "ThreadedPresentation", gpu_threaded_presentation);
    si.SetBoolValue("GPU", "UseSoftwareRendererForReadbacks", gpu_use_software_renderer_for_readbacks);
    si.SetBoolValue("GPU", "TrueColor", gpu_true_color);
    si.SetBoolValue("GPU", "ScaledDithering", gpu_scaled_dithering);
    si.SetStringValue("GPU", "TextureFilter", GetTextureFilterName(gpu_texture_filter));
    si.SetStringValue("GPU", "DownsampleMode", GetDownsampleModeName(gpu_downsample_mode));
    si.SetBoolValue("GPU", "DisableInterlacing", gpu_disable_interlacing);
    si.SetBoolValue("GPU", "ForceNTSCTimings", gpu_force_ntsc_timings);
    si.SetBoolValue("GPU", "WidescreenHack", gpu_widescreen_hack);
    si.SetBoolValue("GPU", "ChromaSmoothing24Bit", gpu_24bit_chroma_smoothing);
    si.SetBoolValue("GPU", "PGXPEnable", gpu_pgxp_enable);
    si.SetBoolValue("GPU", "PGXPCulling", gpu_pgxp_culling);
    si.SetBoolValue("GPU", "PGXPTextureCorrection", gpu_pgxp_texture_correction);
    si.SetBoolValue("GPU", "PGXPVertexCache", gpu_pgxp_vertex_cache);
    si.SetBoolValue("GPU", "PGXPCPU", gpu_pgxp_cpu);
    si.SetBoolValue("GPU", "PGXPPreserveProjFP", gpu_pgxp_preserve_proj_fp);
    si.SetFloatValue("GPU", "PGXPTolerance", gpu_pgxp_tolerance);
    si.SetBoolValue("GPU", "PGXPDepthBuffer", gpu_pgxp_depth_buffer);
    si.SetFloatValue("GPU", "PGXPDepthClearThreshold", GetPGXPDepthClearThreshold());

    si.SetStringValue("Display", "CropMode", GetDisplayCropModeName(display_crop_mode));
    si.SetIntValue("Display", "ActiveStartOffset", display_active_start_offset);
    si.SetIntValue("Display", "ActiveEndOffset", display_active_end_offset);
    si.SetIntValue("Display", "LineStartOffset", display_line_start_offset);
    si.SetIntValue("Display", "LineEndOffset", display_line_end_offset);
    si.SetBoolValue("Display", "Force4_3For24Bit", display_force_4_3_for_24bit);
    si.SetStringValue("Display", "AspectRatio", GetDisplayAspectRatioName(display_aspect_ratio));
    si.SetIntValue("Display", "CustomAspectRatioNumerator", display_aspect_ratio_custom_numerator);
    si.GetIntValue("Display", "CustomAspectRatioDenominator", display_aspect_ratio_custom_denominator);
    si.SetBoolValue("Display", "LinearFiltering", display_linear_filtering);
    si.SetBoolValue("Display", "IntegerScaling", display_integer_scaling);
    si.SetBoolValue("Display", "Stretch", display_stretch);
    si.SetBoolValue("Display", "PostProcessing", display_post_processing);
    si.SetBoolValue("Display", "ShowOSDMessages", display_show_osd_messages);
    si.SetBoolValue("Display", "ShowFPS", display_show_fps);
    si.SetBoolValue("Display", "ShowVPS", display_show_vps);
    si.SetBoolValue("Display", "ShowSpeed", display_show_speed);
    si.SetBoolValue("Display", "ShowResolution", display_show_resolution);
    si.SetBoolValue("Display", "DisplayAllFrames", display_all_frames);
    si.SetBoolValue("Display", "VSync", video_sync_enabled);
    if (display_post_process_chain.empty())
        si.DeleteValue("Display", "PostProcessChain");
    else
        si.SetStringValue("Display", "PostProcessChain", display_post_process_chain.c_str());
    si.SetFloatValue("Display", "MaxFPS", display_max_fps);

    si.SetBoolValue("CDROM", "ReadThread", cdrom_read_thread);
    si.SetBoolValue("CDROM", "RegionCheck", cdrom_region_check);
    si.SetBoolValue("CDROM", "LoadImageToRAM", cdrom_load_image_to_ram);
    si.SetBoolValue("CDROM", "MuteCDAudio", cdrom_mute_cd_audio);
    si.SetIntValue("CDROM", "ReadSpeedup", cdrom_read_speedup);
    si.SetIntValue("CDROM", "SeekSpeedup", cdrom_seek_speedup);

    si.SetStringValue("Audio", "Backend", GetAudioBackendName(audio_backend));
    si.SetIntValue("Audio", "OutputVolume", audio_output_volume);
    si.SetIntValue("Audio", "FastForwardVolume", audio_fast_forward_volume);
    si.SetIntValue("Audio", "BufferSize", audio_buffer_size);
    si.SetBoolValue("Audio", "Resampling", audio_resampling);
    si.SetBoolValue("Audio", "OutputMuted", audio_output_muted);
    si.SetBoolValue("Audio", "Sync", audio_sync_enabled);
    si.SetBoolValue("Audio", "DumpOnBoot", audio_dump_on_boot);

    si.SetIntValue("Hacks", "DMAMaxSliceTicks", dma_max_slice_ticks);
    si.SetIntValue("Hacks", "DMAHaltTicks", dma_halt_ticks);
    si.SetIntValue("Hacks", "GPUFIFOSize", gpu_fifo_size);
    si.SetIntValue("Hacks", "GPUMaxRunAhead", gpu_max_run_ahead);

    si.SetBoolValue("BIOS", "PatchTTYEnable", bios_patch_tty_enable);
    si.SetBoolValue("BIOS", "PatchFastBoot", bios_patch_fast_boot);

    for (u32 i = 0; i < NUM_CONTROLLER_AND_CARD_PORTS; i++)
    {
        si.SetStringValue(TinyString::FromFormat("Controller%u", i + 1u), "Type",
                          GetControllerTypeName(controller_types[i]));
    }

    si.SetStringValue("MemoryCards", "Card1Type", GetMemoryCardTypeName(memory_card_types[0]));
    si.SetStringValue("MemoryCards", "Card2Type", GetMemoryCardTypeName(memory_card_types[1]));
    if (!memory_card_paths[0].empty())
        si.SetStringValue("MemoryCards", "Card1Path", memory_card_paths[0].c_str());
    else
        si.DeleteValue("MemoryCards", "Card1Path");

    if (!memory_card_paths[1].empty())
        si.SetStringValue("MemoryCards", "Card2Path", memory_card_paths[1].c_str());
    else
        si.DeleteValue("MemoryCards", "Card2Path");

    if (!memory_card_directory.empty())
        si.SetStringValue("MemoryCards", "Directory", memory_card_directory.c_str());
    else
        si.DeleteValue("MemoryCards", "Directory");
    si.SetBoolValue("MemoryCards", "UsePlaylistTitle", memory_card_use_playlist_title);

    si.SetStringValue("ControllerPorts", "MultitapMode", GetMultitapModeName(multitap_mode));

    si.SetStringValue("Logging", "LogLevel", GetLogLevelName(log_level));
    si.SetStringValue("Logging", "LogFilter", log_filter.c_str());
    si.SetBoolValue("Logging", "LogToConsole", log_to_console);
    si.SetBoolValue("Logging", "LogToDebug", log_to_debug);
    si.SetBoolValue("Logging", "LogToWindow", log_to_window);
    si.SetBoolValue("Logging", "LogToFile", log_to_file);

    si.SetBoolValue("Debug", "ShowVRAM", debugging.show_vram);
    si.SetBoolValue("Debug", "DumpCPUToVRAMCopies", debugging.dump_cpu_to_vram_copies);
    si.SetBoolValue("Debug", "DumpVRAMToCPUCopies", debugging.dump_vram_to_cpu_copies);
    si.SetBoolValue("Debug", "ShowGPUState", debugging.show_gpu_state);
    si.SetBoolValue("Debug", "ShowCDROMState", debugging.show_cdrom_state);
    si.SetBoolValue("Debug", "ShowSPUState", debugging.show_spu_state);
    si.SetBoolValue("Debug", "ShowTimersState", debugging.show_timers_state);
    si.SetBoolValue("Debug", "ShowMDECState", debugging.show_mdec_state);
    si.SetBoolValue("Debug", "ShowDMAState", debugging.show_dma_state);

    si.SetBoolValue("TextureReplacements", "EnableVRAMWriteReplacements",
                    texture_replacements.enable_vram_write_replacements);
    si.SetBoolValue("TextureReplacements", "PreloadTextures", texture_replacements.preload_textures);
    si.SetBoolValue("TextureReplacements", "DumpVRAMWrites", texture_replacements.dump_vram_writes);
    si.SetBoolValue("TextureReplacements", "DumpVRAMWriteForceAlphaChannel",
                    texture_replacements.dump_vram_write_force_alpha_channel);
    si.SetIntValue("TextureReplacements", "DumpVRAMWriteWidthThreshold",
                   texture_replacements.dump_vram_write_width_threshold);
    si.SetIntValue("TextureReplacements", "DumpVRAMWriteHeightThreshold",
                   texture_replacements.dump_vram_write_height_threshold);
}

static std::array<const char*, LOGLEVEL_COUNT> s_log_level_names = {
        {"None", "Error", "Warning", "Perf", "Info", "Verbose", "Dev", "Profile", "Debug", "Trace"}};
static std::array<const char*, LOGLEVEL_COUNT> s_log_level_display_names = {
        {TRANSLATABLE("LogLevel", "None"), TRANSLATABLE("LogLevel", "Error"), TRANSLATABLE("LogLevel", "Warning"),
         TRANSLATABLE("LogLevel", "Performance"), TRANSLATABLE("LogLevel", "Information"),
         TRANSLATABLE("LogLevel", "Verbose"), TRANSLATABLE("LogLevel", "Developer"), TRANSLATABLE("LogLevel", "Profile"),
         TRANSLATABLE("LogLevel", "Debug"), TRANSLATABLE("LogLevel", "Trace")}};

std::optional<LOGLEVEL> Settings::ParseLogLevelName(const char* str)
{
    int index = 0;
    for (const char* name : s_log_level_names)
    {
        if (StringUtil::Strcasecmp(name, str) == 0)
            return static_cast<LOGLEVEL>(index);

        index++;
    }

    return std::nullopt;
}

const char* Settings::GetLogLevelName(LOGLEVEL level)
{
    return s_log_level_names[static_cast<int>(level)];
}

const char* Settings::GetLogLevelDisplayName(LOGLEVEL level)
{
    return s_log_level_display_names[static_cast<int>(level)];
}

static std::array<const char*, 4> s_console_region_names = {{"Auto", "NTSC-J", "NTSC-U", "PAL"}};
static std::array<const char*, 4> s_console_region_display_names = {
        {TRANSLATABLE("ConsoleRegion", "Auto-Detect"), TRANSLATABLE("ConsoleRegion", "NTSC-J (Japan)"),
         TRANSLATABLE("ConsoleRegion", "NTSC-U/C (US, Canada)"), TRANSLATABLE("ConsoleRegion", "PAL (Europe, Australia)")}};

std::optional<ConsoleRegion> Settings::ParseConsoleRegionName(const char* str)
{
    int index = 0;
    for (const char* name : s_console_region_names)
    {
        if (StringUtil::Strcasecmp(name, str) == 0)
            return static_cast<ConsoleRegion>(index);

        index++;
    }

    return std::nullopt;
}

const char* Settings::GetConsoleRegionName(ConsoleRegion region)
{
    return s_console_region_names[static_cast<int>(region)];
}

const char* Settings::GetConsoleRegionDisplayName(ConsoleRegion region)
{
    return s_console_region_display_names[static_cast<int>(region)];
}

static std::array<const char*, 4> s_disc_region_names = {{"NTSC-J", "NTSC-U", "PAL", "Other"}};
static std::array<const char*, 4> s_disc_region_display_names = {
        {TRANSLATABLE("DiscRegion", "NTSC-J (Japan)"), TRANSLATABLE("DiscRegion", "NTSC-U/C (US, Canada)"),
         TRANSLATABLE("DiscRegion", "PAL (Europe, Australia)"), TRANSLATABLE("DiscRegion", "Other")}};

std::optional<DiscRegion> Settings::ParseDiscRegionName(const char* str)
{
    int index = 0;
    for (const char* name : s_disc_region_names)
    {
        if (StringUtil::Strcasecmp(name, str) == 0)
            return static_cast<DiscRegion>(index);

        index++;
    }

    return std::nullopt;
}

const char* Settings::GetDiscRegionName(DiscRegion region)
{
    return s_disc_region_names[static_cast<int>(region)];
}

const char* Settings::GetDiscRegionDisplayName(DiscRegion region)
{
    return s_disc_region_display_names[static_cast<int>(region)];
}

static std::array<const char*, 3> s_cpu_execution_mode_names = {{"Interpreter", "CachedInterpreter", "Recompiler"}};
static std::array<const char*, 3> s_cpu_execution_mode_display_names = {
        {TRANSLATABLE("CPUExecutionMode", "Interpreter (Slowest)"),
         TRANSLATABLE("CPUExecutionMode", "Cached Interpreter (Faster)"),
         TRANSLATABLE("CPUExecutionMode", "Recompiler (Fastest)")}};

std::optional<CPUExecutionMode> Settings::ParseCPUExecutionMode(const char* str)
{
    u8 index = 0;
    for (const char* name : s_cpu_execution_mode_names)
    {
        if (StringUtil::Strcasecmp(name, str) == 0)
            return static_cast<CPUExecutionMode>(index);

        index++;
    }

    return std::nullopt;
}

const char* Settings::GetCPUExecutionModeName(CPUExecutionMode mode)
{
    return s_cpu_execution_mode_names[static_cast<u8>(mode)];
}

const char* Settings::GetCPUExecutionModeDisplayName(CPUExecutionMode mode)
{
    return s_cpu_execution_mode_display_names[static_cast<u8>(mode)];
}

static std::array<const char*, static_cast<u32>(CPUFastmemMode::Count)> s_cpu_fastmem_mode_names = {
        {"Disabled", "MMap", "LUT"}};
static std::array<const char*, static_cast<u32>(CPUFastmemMode::Count)> s_cpu_fastmem_mode_display_names = {
        {TRANSLATABLE("CPUFastmemMode", "Disabled (Slowest)"),
         TRANSLATABLE("CPUFastmemMode", "MMap (Hardware, Fastest, 64-Bit Only)"),
         TRANSLATABLE("CPUFastmemMode", "LUT (Faster)")}};

std::optional<CPUFastmemMode> Settings::ParseCPUFastmemMode(const char* str)
{
    u8 index = 0;
    for (const char* name : s_cpu_fastmem_mode_names)
    {
        if (StringUtil::Strcasecmp(name, str) == 0)
            return static_cast<CPUFastmemMode>(index);

        index++;
    }

    return std::nullopt;
}

const char* Settings::GetCPUFastmemModeName(CPUFastmemMode mode)
{
    return s_cpu_fastmem_mode_names[static_cast<u8>(mode)];
}

const char* Settings::GetCPUFastmemModeDisplayName(CPUFastmemMode mode)
{
    return s_cpu_fastmem_mode_display_names[static_cast<u8>(mode)];
}

static constexpr auto s_gpu_renderer_names = make_array(
#ifdef WIN32
        "D3D11",
#endif
        "Vulkan", "OpenGL", "Software");
static constexpr auto s_gpu_renderer_display_names = make_array(
#ifdef WIN32
        TRANSLATABLE("GPURenderer", "Hardware (D3D11)"),
#endif
        TRANSLATABLE("GPURenderer", "Hardware (Vulkan)"), TRANSLATABLE("GPURenderer", "Hardware (OpenGL)"),
        TRANSLATABLE("GPURenderer", "Software"));

std::optional<GPURenderer> Settings::ParseRendererName(const char* str)
{
    int index = 0;
    for (const char* name : s_gpu_renderer_names)
    {
        if (StringUtil::Strcasecmp(name, str) == 0)
            return static_cast<GPURenderer>(index);

        index++;
    }

    return std::nullopt;
}

const char* Settings::GetRendererName(GPURenderer renderer)
{
    return s_gpu_renderer_names[static_cast<int>(renderer)];
}

const char* Settings::GetRendererDisplayName(GPURenderer renderer)
{
    return s_gpu_renderer_display_names[static_cast<int>(renderer)];
}

static constexpr auto s_texture_filter_names =
        make_array("Nearest", "Bilinear", "BilinearBinAlpha", "JINC2", "JINC2BinAlpha", "xBR", "xBRBinAlpha");
static constexpr auto s_texture_filter_display_names =
        make_array(TRANSLATABLE("GPUTextureFilter", "Nearest-Neighbor"), TRANSLATABLE("GPUTextureFilter", "Bilinear"),
                   TRANSLATABLE("GPUTextureFilter", "Bilinear (No Edge Blending)"), TRANSLATABLE("GPUTextureFilter", "JINC2"),
                   TRANSLATABLE("GPUTextureFilter", "JINC2 (No Edge Blending)"), TRANSLATABLE("GPUTextureFilter", "xBR"),
                   TRANSLATABLE("GPUTextureFilter", "xBR (No Edge Blending)"));

std::optional<GPUTextureFilter> Settings::ParseTextureFilterName(const char* str)
{
    int index = 0;
    for (const char* name : s_texture_filter_names)
    {
        if (StringUtil::Strcasecmp(name, str) == 0)
            return static_cast<GPUTextureFilter>(index);

        index++;
    }

    return std::nullopt;
}

const char* Settings::GetTextureFilterName(GPUTextureFilter filter)
{
    return s_texture_filter_names[static_cast<int>(filter)];
}

const char* Settings::GetTextureFilterDisplayName(GPUTextureFilter filter)
{
    return s_texture_filter_display_names[static_cast<int>(filter)];
}

static constexpr auto s_downsample_mode_names = make_array("Disabled", "Box", "Adaptive");
static constexpr auto s_downsample_mode_display_names = make_array(
        TRANSLATABLE("GPUDownsampleMode", "Disabled"), TRANSLATABLE("GPUDownsampleMode", "Box (Downsample 3D/Smooth All)"),
        TRANSLATABLE("GPUDownsampleMode", "Adaptive (Preserve 3D/Smooth 2D)"));

std::optional<GPUDownsampleMode> Settings::ParseDownsampleModeName(const char* str)
{
    int index = 0;
    for (const char* name : s_downsample_mode_names)
    {
        if (StringUtil::Strcasecmp(name, str) == 0)
            return static_cast<GPUDownsampleMode>(index);

        index++;
    }

    return std::nullopt;
}

const char* Settings::GetDownsampleModeName(GPUDownsampleMode mode)
{
    return s_downsample_mode_names[static_cast<int>(mode)];
}

const char* Settings::GetDownsampleModeDisplayName(GPUDownsampleMode mode)
{
    return s_downsample_mode_display_names[static_cast<int>(mode)];
}

static std::array<const char*, 3> s_display_crop_mode_names = {{"None", "Overscan", "Borders"}};
static std::array<const char*, 3> s_display_crop_mode_display_names = {
        {TRANSLATABLE("DisplayCropMode", "None"), TRANSLATABLE("DisplayCropMode", "Only Overscan Area"),
         TRANSLATABLE("DisplayCropMode", "All Borders")}};

std::optional<DisplayCropMode> Settings::ParseDisplayCropMode(const char* str)
{
    int index = 0;
    for (const char* name : s_display_crop_mode_names)
    {
        if (StringUtil::Strcasecmp(name, str) == 0)
            return static_cast<DisplayCropMode>(index);

        index++;
    }

    return std::nullopt;
}

const char* Settings::GetDisplayCropModeName(DisplayCropMode crop_mode)
{
    return s_display_crop_mode_names[static_cast<int>(crop_mode)];
}

const char* Settings::GetDisplayCropModeDisplayName(DisplayCropMode crop_mode)
{
    return s_display_crop_mode_display_names[static_cast<int>(crop_mode)];
}

static std::array<const char*, static_cast<size_t>(DisplayAspectRatio::Count)> s_display_aspect_ratio_names = {
        {TRANSLATABLE("DisplayAspectRatio", "Auto (Game Native)"), TRANSLATABLE("DisplayAspectRatio", "Auto (Match Window)"),
         TRANSLATABLE("DisplayAspectRatio", "Custom"), "4:3", "16:9", "19:9", "20:9", "PAR 1:1"}};
static constexpr std::array<float, static_cast<size_t>(DisplayAspectRatio::Count)> s_display_aspect_ratio_values = {
        {-1.0f, -1.0f, -1.0f, 4.0f / 3.0f, 16.0f / 9.0f, 19.0f / 9.0f, 20.0f / 9.0f, -1.0f}};

std::optional<DisplayAspectRatio> Settings::ParseDisplayAspectRatio(const char* str)
{
    int index = 0;
    for (const char* name : s_display_aspect_ratio_names)
    {
        if (StringUtil::Strcasecmp(name, str) == 0)
            return static_cast<DisplayAspectRatio>(index);

        index++;
    }

    return std::nullopt;
}

const char* Settings::GetDisplayAspectRatioName(DisplayAspectRatio ar)
{
    return s_display_aspect_ratio_names[static_cast<int>(ar)];
}

float Settings::GetDisplayAspectRatioValue() const
{
    switch (display_aspect_ratio)
    {
        case DisplayAspectRatio::MatchWindow:
        {
            const HostDisplay* display = g_host_interface->GetDisplay();
            if (!display)
                return s_display_aspect_ratio_values[static_cast<int>(DEFAULT_DISPLAY_ASPECT_RATIO)];

            const u32 width = display->GetWindowWidth();
            const u32 height = display->GetWindowHeight() - display->GetDisplayTopMargin();
            return static_cast<float>(width) / static_cast<float>(height);
        }

        case DisplayAspectRatio::Custom:
        {
            return static_cast<float>(display_aspect_ratio_custom_numerator) /
                   static_cast<float>(display_aspect_ratio_custom_denominator);
        }

        default:
        {
            return s_display_aspect_ratio_values[static_cast<int>(display_aspect_ratio)];
        }
    }
}

static std::array<const char*, 3> s_audio_backend_names = {{
                                                                   "Null",
                                                                   "Cubeb",
#ifndef ANDROID
                                                                   "SDL",
#else
                                                                   "OpenSLES",
#endif
                                                           }};
static std::array<const char*, 3> s_audio_backend_display_names = {{
                                                                           TRANSLATABLE("AudioBackend", "Null (No Output)"),
                                                                           TRANSLATABLE("AudioBackend", "Cubeb"),
#ifndef ANDROID
                                                                           TRANSLATABLE("AudioBackend", "SDL"),
#else
                                                                           TRANSLATABLE("AudioBackend", "OpenSL ES"),
#endif
                                                                   }};

std::optional<AudioBackend> Settings::ParseAudioBackend(const char* str)
{
    int index = 0;
    for (const char* name : s_audio_backend_names)
    {
        if (StringUtil::Strcasecmp(name, str) == 0)
            return static_cast<AudioBackend>(index);

        index++;
    }

    return std::nullopt;
}

const char* Settings::GetAudioBackendName(AudioBackend backend)
{
    return s_audio_backend_names[static_cast<int>(backend)];
}

const char* Settings::GetAudioBackendDisplayName(AudioBackend backend)
{
    return s_audio_backend_display_names[static_cast<int>(backend)];
}

static std::array<const char*, 7> s_controller_type_names = {
        {"None", "DigitalController", "AnalogController", "AnalogJoystick", "NamcoGunCon", "PlayStationMouse", "NeGcon"}};
static std::array<const char*, 7> s_controller_display_names = {
        {TRANSLATABLE("ControllerType", "None"), TRANSLATABLE("ControllerType", "Digital Controller"),
         TRANSLATABLE("ControllerType", "Analog Controller (DualShock)"), TRANSLATABLE("ControllerType", "Analog Joystick"),
         TRANSLATABLE("ControllerType", "Namco GunCon"), TRANSLATABLE("ControllerType", "PlayStation Mouse"),
         TRANSLATABLE("ControllerType", "NeGcon")}};

std::optional<ControllerType> Settings::ParseControllerTypeName(const char* str)
{
    int index = 0;
    for (const char* name : s_controller_type_names)
    {
        if (StringUtil::Strcasecmp(name, str) == 0)
            return static_cast<ControllerType>(index);

        index++;
    }

    return std::nullopt;
}

const char* Settings::GetControllerTypeName(ControllerType type)
{
    return s_controller_type_names[static_cast<int>(type)];
}

const char* Settings::GetControllerTypeDisplayName(ControllerType type)
{
    return s_controller_display_names[static_cast<int>(type)];
}

static std::array<const char*, 6> s_memory_card_type_names = {
        {"None", "Shared", "PerGame", "PerGameTitle", "PerGameFileTitle", "NonPersistent"}};
static std::array<const char*, 6> s_memory_card_type_display_names = {
        {TRANSLATABLE("MemoryCardType", "No Memory Card"), TRANSLATABLE("MemoryCardType", "Shared Between All Games"),
         TRANSLATABLE("MemoryCardType", "Separate Card Per Game (Game Code)"),
         TRANSLATABLE("MemoryCardType", "Separate Card Per Game (Game Title)"),
         TRANSLATABLE("MemoryCardType", "Separate Card Per Game (File Title)"),
         TRANSLATABLE("MemoryCardType", "Non-Persistent Card (Do Not Save)")}};

std::optional<MemoryCardType> Settings::ParseMemoryCardTypeName(const char* str)
{
    int index = 0;
    for (const char* name : s_memory_card_type_names)
    {
        if (StringUtil::Strcasecmp(name, str) == 0)
            return static_cast<MemoryCardType>(index);

        index++;
    }

    return std::nullopt;
}

const char* Settings::GetMemoryCardTypeName(MemoryCardType type)
{
    return s_memory_card_type_names[static_cast<int>(type)];
}

const char* Settings::GetMemoryCardTypeDisplayName(MemoryCardType type)
{
    return s_memory_card_type_display_names[static_cast<int>(type)];
}

static std::array<const char*, 4> s_multitap_enable_mode_names = {{"Disabled", "Port1Only", "Port2Only", "BothPorts"}};
static std::array<const char*, 4> s_multitap_enable_mode_display_names = {
        {TRANSLATABLE("MultitapMode", "Disabled"), TRANSLATABLE("MultitapMode", "Enable on Port 1 Only"),
         TRANSLATABLE("MultitapMode", "Enable on Port 2 Only"), TRANSLATABLE("MultitapMode", "Enable on Ports 1 and 2")}};

std::optional<MultitapMode> Settings::ParseMultitapModeName(const char* str)
{
    u32 index = 0;
    for (const char* name : s_multitap_enable_mode_names)
    {
        if (StringUtil::Strcasecmp(name, str) == 0)
            return static_cast<MultitapMode>(index);

        index++;
    }

    return std::nullopt;
}

const char* Settings::GetMultitapModeName(MultitapMode mode)
{
    return s_multitap_enable_mode_names[static_cast<size_t>(mode)];
}

const char* Settings::GetMultitapModeDisplayName(MultitapMode mode)
{
    return s_multitap_enable_mode_display_names[static_cast<size_t>(mode)];
}