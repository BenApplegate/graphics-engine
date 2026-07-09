//// Author: Benjamin Applegate

#include <stdio.h>
#include <stdlib.h>

#include "component.h"
#include "engine.h"
#include "../components/test_component.h"

struct component* get_new_component_of_type(enum COMPONENT_TYPE type) {
    switch (type) {
        case TEST:
            return test_component_new();
            break;
        default:
            if (ENGINE_FLAGS.verbose) fprintf(stderr, "WARNING: could not load component type: %i\n", type);
            return nullptr;
    }
}

void component_init(struct component* component){
    if (component->interface->init != nullptr) {
        component->interface->init(component->data);
    }
}

void component_update(struct component* component){
    if (component->interface->update != nullptr) {
        component->interface->update(component->data);
    }
}

void component_render(struct component* component){
    if (component->interface->render != nullptr) {
        component->interface->render(component->data);
    }
}

void component_free(struct component* component){
    if (component->interface->free != nullptr) {
        component->interface->free(component->data);
    }

    free(component);
}

void component_draw_debug_ui(struct component* component){
    if (component->interface->draw_debug_ui != nullptr) {
        component->interface->draw_debug_ui(component->data);
    }
}

int component_save_to_file(struct component* component, FILE* file) {
    if (fwrite(&component->type, sizeof(enum COMPONENT_TYPE), 1, file) != 1) return -1;
    if (component->interface->save_to_file != nullptr) {
        return component->interface->save_to_file(component->data, file);
    }
    return 0;
}

struct component* component_load_from_file(FILE* file) {
    enum COMPONENT_TYPE type;
    if (fread(&type, sizeof(enum COMPONENT_TYPE), 1, file) != 1) return nullptr;

    struct component* comp = get_new_component_of_type(type);
    if (comp->interface->load_from_file != nullptr) {
        if (comp->interface->load_from_file(comp->data, file) != 0) {
            component_free(comp);
            return nullptr;
        }
    }
    else {
        //If there is no load function, we default init it so there wont be issues
        component_init(comp);
    }

    return comp;
}