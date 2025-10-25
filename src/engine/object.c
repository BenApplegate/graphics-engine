#include "object.h"

#include <stdlib.h>
#include <string.h>

#include "cimgui/cimgui.h"

struct object* object_new(const char* name, struct object* parent) {
    struct object* new_object = malloc(sizeof(struct object));
    new_object->name = strdup(name);
    new_object->parent = parent;

    new_object->children = list_vp_new(5);
    new_object->components = list_vp_new(5);

    new_object->show_debug_window = false;

    return new_object;
}

void object_free(struct object* object) {
    // First free children
    for (int i = 0; i < object->children.size; i++) {
        object_free(list_vp_at(&object->children, i));
    }
    list_vp_delete(&object->children);

    // TODO: Call free functions on components when they exist
    list_vp_delete(&object->components);

    free(object->name);
    free(object);
}

void object_update(const struct object* object) {
    for (int i = 0 ; i < object->children.size; i++) {
        object_update(list_vp_at(&object->children, i));
    }

    for (int i = 0; i < object->components.size; i++) {
    // TODO: Update components once they exist

    }
}

void object_render(const struct object* object) {
    for (int i = 0; i < object->children.size; i++) {
        object_render(list_vp_at(&object->children, i));
    }

    for (int i = 0; i < object->components.size; i++) {
        // TODO: Render components once they exist

    }
}

void object_add_new_child(struct object* object, const char* child_name) {
    list_vp_add(&object->children, object_new(child_name, object));
}

void object_add_child_modal_ui(struct object* object) {
    if (igButton("Add Child", (ImVec2){0, 0})) {
        igOpenPopup_Str("Add Child Object", 0);
    }

    //Draw popup
    ImVec2 center;
    ImGuiViewport_GetCenter(&center, igGetMainViewport());
    igSetNextWindowPos(center, ImGuiCond_Appearing, (ImVec2){0.5f, 0.5f});

    if (igBeginPopupModal("Add Child Object", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        static char object_name[32] = "New Object";

        igInputText("Object Name", object_name, 32, ImGuiInputTextFlags_None, nullptr, nullptr);

        //Cancel and close button
        if (igButton("Add New Child", (ImVec2){0,0})) {
            object_add_new_child(object, object_name);
            igCloseCurrentPopup();
        }
        igSameLine(0, 20);
        if (igButton("Close", (ImVec2){0, 0})) { igCloseCurrentPopup(); }

        igEndPopup();
    }
}

void object_draw_scene_tree_node(struct object* object) {

    if (igTreeNodeEx_Str(object->name, ImGuiTreeNodeFlags_DrawLinesFull)) {
        object_add_child_modal_ui(object);
        igSameLine(0, 20);

        if (igButton("Open Object Window", (ImVec2){0,0})) {
            object->show_debug_window = true;
        }

        for (int i = 0; i < object->children.size; i++) {
            igPushID_Int(i);
            object_draw_scene_tree_node(object->children.data[i]);
            igPopID();
        }
        igTreePop();
    }

}

void object_draw_debug_window(struct object* object) {
    for (int i = 0; i < object->children.size; i++) {
        igPushID_Int(i);
        object_draw_debug_window(object->children.data[i]);
        igPopID();
    }

    if (!object->show_debug_window) return;

    char window_title[64];
    sprintf(window_title, "%s [%p]", object->name, object);

    if (!igBegin(window_title, &object->show_debug_window, 0)){
        igEnd();
        return;
    }

    igText("Children: %i\tChild Capacity: %i", object->children.size, object->children.capacity);
    igText("Components: %i\tComponent Capacity: %i", object->components.size, object->components.capacity);

    igSeparatorText("Components");

    igSeparatorText("Children");
    object_add_child_modal_ui(object);

    for (int i = 0; i < object->children.size; i++) {
        igPushID_Int(i);
        object_draw_scene_tree_node(object->children.data[i]);
        igPopID();
    }

    igEnd();
}