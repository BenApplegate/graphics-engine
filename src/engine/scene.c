/// Author: Benjamin Applegate

#include "scene.h"

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cimgui/cimgui.h>

#include "object.h"

struct scene* scene_new(const char* name) {
    struct scene* new_scene = malloc(sizeof(struct scene));
    new_scene->name = strdup(name);
    new_scene->objects_list = list_vp_new(10);

    return new_scene;
}

void scene_free(struct scene* scene) {
    //Free all scene objects
    for (int i = 0; i < scene->objects_list.size; i++) {
        object_free(scene->objects_list.data[i]);
    }

    //Free scene data
    list_vp_delete(&scene->objects_list);
    free(scene->name);
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

void scene_add_object(struct scene* scene, struct object* object) {
    list_vp_add(&scene->objects_list, object);
}

void scene_draw_debug_ui(struct scene* scene) {
    //Construct name for window
    //TODO: Dont use hardcoded length for window name, instead calculate needed length
    char window_name[256];
    strcpy(window_name, "Scene: ");
    strcat(window_name, scene->name);
    igBegin(window_name, nullptr, ImGuiWindowFlags_None);

    igBeginMenuBar();
    if (igMenuItem_Bool("Save", nullptr, false, true)) {
        scene_save_to_file(scene, "test.scn");
    }

    //Display number of objects in scene
    igText("Objects: %i", scene->objects_list.size);
    igText("Current Objects Capacity: %i", scene->objects_list.capacity);

    igSeparator();

    //Add popup to add scene item
    ImVec2 button_size = {0, 0};
    if (igButton("Add Object", button_size)) {
        igOpenPopup_Str("Add Scene Object", ImGuiPopupFlags_None);
    }

    ImVec2 center;
    ImGuiViewport_GetCenter(&center, igGetMainViewport());
    igSetNextWindowPos(center, ImGuiCond_Appearing, (ImVec2){0.5f, 0.5f});

    if (igBeginPopupModal("Add Scene Object", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        static char object_name[32] = "New Object";

        igInputText("Object Name", object_name, 32, ImGuiInputTextFlags_None, nullptr, nullptr);

        //Cancel and close button
        if (igButton("Add Object", button_size)) {
            scene_add_new_object(scene, object_name);
            igCloseCurrentPopup();
        }
        igSameLine(0, 20);
        if (igButton("Close", button_size)) { igCloseCurrentPopup(); }

        igEndPopup();
    }

    //Draw Scene Tree
    if (igTreeNodeEx_Str("Scene Tree", ImGuiTreeNodeFlags_DrawLinesFull)) {
        for (int i = 0; i < scene->objects_list.size; i++) {
            igPushID_Int(i);
            object_draw_scene_tree_node(scene->objects_list.data[i]);
            igPopID();
        }

        igTreePop();
    }
    igEnd();

    //Now we draw windows for any objects inside the scene
    for (int i = 0; i < scene->objects_list.size; i++) {
        igPushID_Int(i);
        object_draw_debug_window(scene->objects_list.data[i]);
        igPopID();
    }
}

struct scene* scene_create_test_scene() {
    struct scene* scene = scene_new("Test Scene");

    struct object* test_object = object_new("Test Object", nullptr);
    object_add_new_component_of_type(test_object, TEST);

    scene_add_object(scene, test_object);
    return scene;
}