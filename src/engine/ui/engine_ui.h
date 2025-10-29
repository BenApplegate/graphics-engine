/// Author: Benjamin Applegate
/// Handles drawing UI for the engine including the menu bar and scene/object windows

#pragma once

struct ui_flags {
    bool show_scene_windows;
    bool show_imgui_demo_window;
};

/// Flags that control what windows get drawn
extern struct ui_flags UI_FLAGS;

/// Draws the engine Menu Bar
void ui_draw_engine_menu_bar();

/// Draws Scene windows and imgui demo window
void ui_draw_engine_ui();