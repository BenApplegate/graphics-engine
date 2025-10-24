#include "engine_ui.h"

#include "../../util/list_vp.h"

#include <cimgui/cimgui.h>

#include "../engine.h"
#include "../scene.h"

struct ui_flags UI_FLAGS = {
    false,
    false
};

void ui_draw_engine_menu_bar() {
    if (igBeginMainMenuBar()) {

        if (igBeginMenu("Scene", true)) {
            if (igMenuItem_Bool("Load Test Scene", "", false, true)) {
                //TODO: Create actual test scene and load it
                engine_load_new_scene("Test Scene");
            }

            igEndMenu();
        }

        if (igBeginMenu("Window", true)) {
            igMenuItem_BoolPtr("Show Scene Windows", "", &UI_FLAGS.show_scene_windows, true);
            if (ENGINE_FLAGS.show_imgui_windows >= 2) igMenuItem_BoolPtr("Show ImGui Demo Window", "", &UI_FLAGS.show_imgui_demo_window, true);
            igEndMenu();
        }

        igEndMainMenuBar();
    }
}

void ui_draw_engine_ui() {
    if (UI_FLAGS.show_scene_windows) {
        struct list_vp* scenes = engine_get_scenes();
        for (int i = 0; i < scenes->size; i++) {
            scene_draw_debug_ui(scenes->data[i]);
        }
    }

    if (UI_FLAGS.show_imgui_demo_window) {
        igShowDemoWindow(0);
    }
}