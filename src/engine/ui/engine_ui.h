#pragma once

struct ui_flags {
    bool show_scene_windows;
    bool show_imgui_demo_window;
};

extern struct ui_flags UI_FLAGS;

void ui_draw_engine_menu_bar();

void ui_draw_engine_ui();