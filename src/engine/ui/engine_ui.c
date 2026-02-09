/// Author: Benjamin Applegate

#include <cimgui/cimgui.h>

#include "engine_ui.h"
#include "../../util/list_vp.h"
#include "../engine.h"
#include "../scene.h"

struct ui_flags UI_FLAGS = {
    true,
    false
};

void ui_draw_engine_menu_bar() {
    if (igBeginMainMenuBar()) {

        if (igBeginMenu("Scene", true)) {
            if (igMenuItem_Bool("Load Scene from file", "", false, true)) {
                struct scene* new_scene = scene_load_from_file("test.scn");
                if (new_scene != nullptr) engine_load_scene(new_scene);
            }
            if (igMenuItem_Bool("Load Test Scene", "", false, true)) {
                engine_load_scene(scene_create_test_scene());
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
        igShowDemoWindow(nullptr);
    }
}