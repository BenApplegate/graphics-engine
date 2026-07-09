/// Author: Benjamin Applegate

#include "object.h"

#include <stdlib.h>
#include <string.h>

#include "cimgui/cimgui.h"
#include "component.h"

struct object* object_new(const char* name, struct object* parent) {
    struct object* new_object = malloc(sizeof(struct object));
    new_object->name = strdup(name);
    new_object->parent = parent;
    new_object->show_debug_window = false;

    new_object->children = list_vp_new(5);
    new_object->components = list_vp_new(5);

    return new_object;
}

void object_free(struct object* object) {
    //Free children then delete list of children
    for (int i = 0; i < object->children.size; i++) {
        object_free(list_vp_at(&object->children, i));
    }
    list_vp_delete(&object->children);

    //Free components then delete list of components
    for (int i = 0; i < object->components.size; i++) {
        component_free(list_vp_at(&object->components, i));
    }
    list_vp_delete(&object->components);

    //Finally free the object itself
    free(object->name);
    free(object);
}

void object_update(const struct object* object) {
    //TODO: Decide if we should update top down or bottom up

    for (int i = 0 ; i < object->children.size; i++) {
        object_update(list_vp_at(&object->children, i));
    }

    for (int i = 0; i < object->components.size; i++) {
        component_update(list_vp_at(&object->components, i));
    }
}

void object_render(const struct object* object) {
    //TODO: Decide if we should render top down or bottom up

    for (int i = 0; i < object->children.size; i++) {
        object_render(list_vp_at(&object->children, i));
    }

    for (int i = 0; i < object->components.size; i++) {
        component_render(list_vp_at(&object->components, i));
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
    if (igButton("Add test component", (ImVec2){0, 0})) {
        object_add_new_component_of_type(object, TEST);
    }

    for (int i = 0; i < object->components.size; i++) {
        igPushID_Int(i);
        if (igCollapsingHeader_BoolPtr(((struct component*)object->components.data[i])->interface->printable_name, nullptr, 0)) {
            component_draw_debug_ui(object->components.data[i]);
        }
        igPopID();
    }

    igSeparatorText("Children");
    object_add_child_modal_ui(object);

    for (int i = 0; i < object->children.size; i++) {
        igPushID_Int(i);
        object_draw_scene_tree_node(object->children.data[i]);
        igPopID();
    }

    igEnd();
}

void object_add_new_component_of_type(struct object* object, enum COMPONENT_TYPE type) {
    //Request new instance of component
    struct component* new_component = get_new_component_of_type(type);
    list_vp_add(&object->components, new_component);

    //Initialize new component
    component_init(new_component);
}

int object_save_to_file(struct object* object, FILE* file) {
    //Write object name to file
    size_t namelen = strlen(object->name);
    if (fwrite(&namelen, sizeof(size_t), 1, file) != 1) return -1;
    if (fwrite(object->name, sizeof(char), namelen, file) != namelen) return -1;

    //Write number of components then write components
    if (fwrite(&object->components.size, sizeof(size_t), 1, file) != 1) return -1;
    for (int i = 0; i < object->components.size; i++) {
        if (component_save_to_file(object->components.data[i], file)) return -1;
    }

    //Write number of children, then write children to file
    if (fwrite(&object->children.size, sizeof(size_t), 1, file) != 1) return -1;
    for (int i = 0; i < object->children.size; i++) {
        if (object_save_to_file(object->children.data[i], file)) return -1;
    }

    return 0;
}

struct object* object_load_from_file(FILE* file, struct object* parent) {
    size_t namelen;
    if (fread(&namelen, sizeof(size_t), 1, file) != 1) return nullptr;
    char* name = (char*)malloc(namelen + 1);
    name[namelen] = '\0';
    if (fread(name, sizeof(char), namelen, file) != namelen) { free(name); return nullptr;}

    struct object* new_object = object_new(name, parent);
    free(name);

    //Load object's components
    size_t component_count;
    if (fread(&component_count, sizeof(size_t), 1, file) != 1) {object_free(new_object); return nullptr;}
    for (int i = 0; i < component_count; i++) {
        struct component* comp = component_load_from_file(file);
        if (comp == nullptr) { object_free(new_object); return nullptr; }
        list_vp_add(&new_object->components, comp);
    }

    //Load object's children
    size_t children_count;
    if (fread(&children_count, sizeof(size_t), 1, file) != 1) {object_free(new_object); return nullptr;}
    for (int i = 0; i < children_count; i++) {
        struct object* child = object_load_from_file(file, new_object);
        if (child == nullptr) {object_free(new_object); return nullptr;}
        list_vp_add(&new_object->children, child);
    }

    return new_object;
}