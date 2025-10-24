#include "scene.h"

#include <stdlib.h>
#include <string.h>
#include <cimgui/cimgui.h>

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

void scene_update(struct scene* scene) {

}

void scene_render(struct scene* scene) {

}

void scene_draw_debug_ui(struct scene* scene) {
    static bool window_open;

    char window_name[256];
    strcpy(window_name, "Scene: ");
    strcat(window_name, scene->name);
    igBegin(window_name, &window_open, ImGuiWindowFlags_None);

    igText("Objects: %i", scene->objects_list.size);
    igText("Current Objects Capacity: %i", scene->objects_list.capacity);

    igEnd();
}