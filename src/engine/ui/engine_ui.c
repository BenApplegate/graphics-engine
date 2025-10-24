#include "engine_ui.h"

#include <cimgui/cimgui.h>

void ui_draw_engine_menu_bar() {
    if (igBeginMainMenuBar()) {
        if (igBeginMenu("Window", true)) {
            static bool show_scene_window = true;
            if (igMenuItem_BoolPtr("Show Scene Window", "", &show_scene_window, true)) {

            }

            igEndMenu();
        }

        igEndMainMenuBar();
    }
}