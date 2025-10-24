#include "object.h"

#include <stdlib.h>
#include <string.h>

#include "cimgui/cimgui.h"

struct object* object_new(const char* name, struct object* parent) {
    struct object* new_object = malloc(sizeof(struct object));
    new_object->name = name;
    new_object->parent = parent;

    new_object->children = list_vp_new(5);
    new_object->components = list_vp_new(5);

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

void object_draw_scene_tree_node(const struct object* object) {
    if (igTreeNodeEx_Str(object->name, ImGuiTreeNodeFlags_DrawLinesFull)) {
        for (int i = 0; i < object->children.size; i++) {
            object_draw_scene_tree_node(object->children.data[i]);
        }

        igTreePop();
    }

}