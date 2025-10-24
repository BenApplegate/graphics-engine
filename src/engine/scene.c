#include "scene.h"

#include <stdlib.h>
#include <string.h>
#include <cimgui/cimgui.h>

#include "object.h"

struct scene* scene_new(const char* name) {
    struct scene* new_scene = malloc(sizeof(struct scene));
    new_scene->name = name;
    new_scene->objects_list = list_vp_new(10);

    return new_scene;
}

void scene_free(struct scene* scene) {
    list_vp_delete(&scene->objects_list);
    free(scene);
}

void scene_update(const struct scene* scene) {
    for (int i = 0; i < scene->objects_list.size; i++) {
        object_update(list_vp_at(&scene->objects_list, i));
    }
}

void scene_render(const struct scene* scene) {
    for (int i = 0; i < scene->objects_list.size; i++) {
        object_render(list_vp_at(&scene->objects_list, i));
    }
}

void scene_add_new_object(struct scene* scene, const char* name) {
    list_vp_add(&scene->objects_list, object_new(name, nullptr));
}

void scene_draw_debug_ui(struct scene* scene) {
    static bool window_open;

    char window_name[256];
    strcpy(window_name, "Scene: ");
    strcat(window_name, scene->name);
    igBegin(window_name, &window_open, ImGuiWindowFlags_None);

    igText("Objects: %i", scene->objects_list.size);
    igText("Current Objects Capacity: %i", scene->objects_list.capacity);

    igSeparator();

    //Add popup to add scene item
    ImVec2 size = {200.0f, 30.0f};
    if (igButton("Add Object", size)) {
        igOpenPopup_Str("Add Scene Object", ImGuiPopupFlags_None);
    }

    ImVec2 center;
    ImVec2 pivot = {0.5f, 0.5f};
    ImGuiViewport_GetCenter(&center, igGetMainViewport());
    igSetNextWindowPos(center, ImGuiCond_Appearing, pivot);

    if (igBeginPopupModal("Add Scene Object", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        static char object_name[64] = "New Object";

        igInputText("Object Name", object_name, 64, ImGuiInputTextFlags_None, nullptr, nullptr);

        //Cancel and close button
        if (igButton("Add Object", size)) {
            scene_add_new_object(scene, object_name);
            igCloseCurrentPopup();
        }
        igSameLine(1, 1);
        if (igButton("Close", size)) { igCloseCurrentPopup(); }

        igEndPopup();
    }

    if (igTreeNodeEx_Str("Scene Tree", ImGuiTreeNodeFlags_DrawLinesFull)) {
        for (int i = 0; i < scene->objects_list.size; i++) {
            object_draw_scene_tree_node(scene->objects_list.data[i]);
        }

        igTreePop();
    }



    igEnd();
}