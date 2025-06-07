// Vita3K emulator project
// Copyright (C) 2025 Vita3K team
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#pragma once

#include <util/system.h>

enum ModulesMode {
    AUTOMATIC,
    AUTO_MANUAL,
    MANUAL
};

enum PerformanceOverlayDetail {
    MINIMUM,
    LOW,
    MEDIUM,
    MAXIMUM,
};

enum PerformanceOverlayPosition {
    TOP_LEFT,
    TOP_CENTER,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_CENTER,
    BOTTOM_RIGHT,
};

enum ScreenshotFormat {
    None,
    JPEG,
    PNG,
};

// clang-format off
// Singular options produced in config file
// Order is code(option_type, option_name, option_default, member_name)
// When adding in a new macro for generation, ALL options must be stated.
// All member names starting with "keyboard_" will be considered as key input (See controls_dialog.cpp)
#define CONFIG_INDIVIDUAL(code)                                                                         \
    code(bool, "initial-setup", false, initial_setup)                                                   \
    code(bool, "gdbstub", false, gdbstub)                                                               \
    code(bool, "log-active-shaders", false, log_active_shaders)                                         \
    code(bool, "log-uniforms", false, log_uniforms)                                                     \
    code(bool, "log-compat-warn", false, log_compat_warn)                                               \
    code(bool, "validation-layer", true, validation_layer)                                              \
    code(bool, "pstv-mode", false, pstv_mode)                                                           \
    code(bool, "show-mode", false, show_mode)                                                           \
    code(bool, "demo-mode", false, demo_mode)                                                           \
    code(bool, "show-gui", false, show_gui)                                                             \
    code(bool, "show-info-bar", false, show_info_bar)                                                   \
    code(bool, "apps-list-grid", false, apps_list_grid)                                                 \
    code(bool, "display-system-apps", true, display_system_apps)                                        \
    code(bool, "stretch_the_display_area", false, stretch_the_display_area)                             \
    code(bool, "fullscreen_hd_res_pixel_perfect", false, fullscreen_hd_res_pixel_perfect)               \
    code(bool, "show-live-area-screen", true, show_live_area_screen)                                    \
    code(int, "icon-size", 64, icon_size)                                                               \
    code(bool, "archive-log", false, archive_log)                                                       \
    code(std::string, "backend-renderer", "OpenGL", backend_renderer)                                   \
    code(int, "gpu-idx", 0, gpu_idx)                                                                    \
    code(bool, "high-accuracy", true, high_accuracy)                                                    \
    code(float, "resolution-multiplier", 1.0f, resolution_multiplier)                                   \
    code(bool, "disable-surface-sync", true, disable_surface_sync)                                      \
    code(std::string, "screen-filter", "Bilinear", screen_filter)                                       \
    code(bool, "v-sync", true, v_sync)                                                                  \
    code(int, "anisotropic-filtering", 1, anisotropic_filtering)                                        \
    code(bool, "texture-cache", true, texture_cache)                                                    \
    code(bool, "async-pipeline-compilation", true, async_pipeline_compilation)                          \
    code(bool, "show-compile-shaders", true, show_compile_shaders)                                      \
    code(bool, "hashless-texture-cache", false, hashless_texture_cache)                                 \
    code(bool, "import-textures", false, import_textures)                                               \
    code(bool, "export-textures", false, export_textures)                                               \
    code(bool, "export-as-png", true, export_as_png)                                                    \
    code(bool, "boot-apps-full-screen", false, boot_apps_full_screen)                                   \
    code(std::string, "audio-backend", "SDL", audio_backend)                                            \
    code(int, "audio-volume", 100, audio_volume)                                                        \
    code(bool, "ngs-enable", true, ngs_enable)                                                          \
    code(int, "sys-button", static_cast<int>(SCE_SYSTEM_PARAM_ENTER_BUTTON_CROSS), sys_button)          \
    code(int, "sys-lang", static_cast<int>(SCE_SYSTEM_PARAM_LANG_ENGLISH_US), sys_lang)                 \
    code(int, "sys-date-format", (int)SCE_SYSTEM_PARAM_DATE_FORMAT_MMDDYYYY, sys_date_format)           \
    code(int, "sys-time-format", (int)SCE_SYSTEM_PARAM_TIME_FORMAT_12HOUR, sys_time_format)             \
    code(int, "cpu-pool-size", 10, cpu_pool_size)                                                       \
    code(int, "modules-mode", static_cast<int>(ModulesMode::AUTOMATIC), modules_mode)                   \
    code(int, "delay-background", 4, delay_background)                                                  \
    code(int, "delay-start", 10, delay_start)                                                           \
    code(float, "background-alpha", .300f, background_alpha)                                            \
    code(int, "log-level", 0 /*SPDLOG_LEVEL_TRACE*/, log_level)                                         \
    code(std::string, "cpu-backend", "Dynarmic", cpu_backend)                                           \
    code(bool, "cpu-opt", true, cpu_opt)                                                                \
    code(std::string, "pref-path", std::string{}, pref_path)                                            \
    code(bool, "discord-rich-presence", true, discord_rich_presence)                                    \
    code(bool, "wait-for-debugger", false, wait_for_debugger)                                           \
    code(bool, "color-surface-debug", false, color_surface_debug)                                       \
    code(bool, "show-touchpad-cursor", true, show_touchpad_cursor)                                      \
    code(bool, "performance-overlay", false, performance_overlay)                                       \
    code(int, "performance-overlay-detail", static_cast<int>(MINIMUM), performance_overlay_detail)      \
    code(int, "performance-overlay-position", static_cast<int>(TOP_LEFT), performance_overlay_position) \
    code(int, "screenshot-format", static_cast<int>(JPEG), screenshot_format)                           \
    code(bool, "disable-motion", false, disable_motion)                                                 \
    code(int, "keyboard-button-select", 532, keyboard_button_select) /* ImGuiKey_RightShift */          \
    code(int, "keyboard-button-start", 525, keyboard_button_start) /* ImGuiKey_Enter */                 \
    code(int, "keyboard-button-up", 515, keyboard_button_up) /* ImGuiKey_UpArrow */                     \
    code(int, "keyboard-button-right", 514, keyboard_button_right) /* ImGuiKey_RightArrow */            \
    code(int, "keyboard-button-down", 516, keyboard_button_down) /* ImGuiKey_DownArrow */               \
    code(int, "keyboard-button-left", 513, keyboard_button_left) /* ImGuiKey_LeftArrow */               \
    code(int, "keyboard-button-l1", 562, keyboard_button_l1) /* ImGuiKey_Q */                           \
    code(int, "keyboard-button-r1", 550, keyboard_button_r1) /* ImGuiKey_E */                           \
    code(int, "keyboard-button-l2", 566, keyboard_button_l2) /* ImGuiKey_U */                           \
    code(int, "keyboard-button-r2", 560, keyboard_button_r2) /* ImGuiKey_O */                           \
    code(int, "keyboard-button-l3", 551, keyboard_button_l3) /* ImGuiKey_F */                           \
    code(int, "keyboard-button-r3", 553, keyboard_button_r3) /* ImGuiKey_H */                           \
    code(int, "keyboard-button-triangle", 567, keyboard_button_triangle) /* ImGuiKey_V */               \
    code(int, "keyboard-button-circle", 548, keyboard_button_circle) /* ImGuiKey_C */                   \
    code(int, "keyboard-button-cross", 569, keyboard_button_cross) /* ImGuiKey_X */                     \
    code(int, "keyboard-button-square", 571, keyboard_button_square) /* ImGuiKey_Z */                   \
    code(int, "keyboard-leftstick-left", 546, keyboard_leftstick_left) /* ImGuiKey_A */                 \
    code(int, "keyboard-leftstick-right", 549, keyboard_leftstick_right) /* ImGuiKey_D */               \
    code(int, "keyboard-leftstick-up", 568, keyboard_leftstick_up) /* ImGuiKey_W */                     \
    code(int, "keyboard-leftstick-down", 564, keyboard_leftstick_down) /* ImGuiKey_S */                 \
    code(int, "keyboard-rightstick-left", 555, keyboard_rightstick_left) /* ImGuiKey_J */               \
    code(int, "keyboard-rightstick-right", 557, keyboard_rightstick_right) /* ImGuiKey_L */             \
    code(int, "keyboard-rightstick-up", 554, keyboard_rightstick_up) /* ImGuiKey_I */                   \
    code(int, "keyboard-rightstick-down", 556, keyboard_rightstick_down) /* ImGuiKey_K */               \
    code(int, "keyboard-button-psbutton", 561, keyboard_button_psbutton) /* ImGuiKey_P */               \
    code(int, "keyboard-gui-toggle-gui", 552, keyboard_gui_toggle_gui) /* ImGuiKey_G */                 \
    code(int, "keyboard-gui-fullscreen", 582, keyboard_gui_fullscreen) /* ImGuiKey_F11 */               \
    code(int, "keyboard-gui-toggle-touch", 565, keyboard_gui_toggle_touch) /* ImGuiKey_T */             \
    code(int, "keyboard-toggle-texture-replacement", 0, keyboard_toggle_texture_replacement)            \
    code(int, "keyboard-take-screenshot", 0, keyboard_take_screenshot)                                  \
    code(std::string, "user-id", std::string{}, user_id)                                                \
    code(bool, "user-auto-connect", false, auto_user_login)                                             \
    code(std::string, "user-lang", std::string{}, user_lang)                                            \
    code(bool, "display-info-message", false, display_info_message)                                     \
    code(bool, "show-welcome", true, show_welcome)                                                      \
    code(bool, "check-for-updates", true, check_for_updates)                                            \
    code(int, "file-loading-delay", 0, file_loading_delay)                                              \
    code(bool, "asia-font-support", false, asia_font_support)                                           \
    code(bool, "shader-cache", true, shader_cache)                                                      \
    code(bool, "spirv-shader", false, spirv_shader)                                                     \
    code(bool, "fps-hack", false, fps_hack)                                                             \
    code(uint64_t, "current-ime-lang", 4, current_ime_lang)                                             \
    code(int, "psn-signed-in", false, psn_signed_in)                                                    \
    code(bool, "http-enable", true, http_enable)                                                        \
    code(int, "http-timeout-attempts", 50, http_timeout_attempts)                                       \
    code(int, "http-timeout-sleep-ms", 100, http_timeout_sleep_ms)                                      \
    code(int, "http-read-end-attempts", 10, http_read_end_attempts)                                     \
    code(int, "http-read-end-sleep-ms", 250, http_read_end_sleep_ms)                                    \
    code(bool, "tracy-primitive-impl", false, tracy_primitive_impl)

// Vector members produced in the config file
// Order is code(option_type, option_name, default_value)
// If you are going to implement a dynamic list in the YAML, add it here instead
// When adding in a new macro for generation, ALL options must be stated.
#define CONFIG_VECTOR(code)                                                                             \
    code(std::vector<short>, "controller-binds", std::vector<short>{}, controller_binds)                \
    code(std::vector<int>, "controller-led-color", std::vector<int>{}, controller_led_color)            \
    code(std::vector<std::string>, "lle-modules", std::vector<std::string>{}, lle_modules)              \
    code(std::vector<uint64_t>, "ime-langs", std::vector<uint64_t>{4}, ime_langs)                       \
    code(std::vector<std::string>, "tracy-advanced-profiling-modules", std::vector<std::string>{}, tracy_advanced_profiling_modules)

// Parent macro for easier generation
#define CONFIG_LIST(code)                                                                               \
    CONFIG_INDIVIDUAL(code)                                                                             \
    CONFIG_VECTOR(code)

// clang-format on
