#pragma once

struct ui_flags {
    bool show_scene_windows;
};

extern struct ui_flags UI_FLAGS;

void ui_draw_engine_menu_bar();

void ui_draw_engine_ui();