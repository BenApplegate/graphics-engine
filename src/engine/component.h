#pragma once
#include <stdlib.h>

/// Holds the type of the component, can be used to identify data type of contained info
enum COMPONENT_TYPE {
    TEST
};

struct component_interface {
    const char* printable_name;
    void (*init)(void* data); //This function is called when the component is loaded into the object
    void (*update)(void* data); //This function is called every frame
    void (*render)(void* data); //This function is called every frame after update has ran for the entire scene
    void (*free)(void* data); //This function is called to delete component data
    void (*draw_debug_ui)(void* data); //This function is called to draw this component's imgui UI

};

struct component {
    enum COMPONENT_TYPE type;
    void* data;
    const struct component_interface* interface;
};

struct component* get_new_component_of_type(enum COMPONENT_TYPE type);

inline void component_init(struct component* component){
    if (component->interface->init != nullptr) {
        component->interface->init(component->data);
    }
}

inline void component_update(struct component* component){
    if (component->interface->update != nullptr) {
        component->interface->update(component->data);
    }
}

inline void component_render(struct component* component){
    if (component->interface->render != nullptr) {
        component->interface->render(component->data);
    }
}

/// Call the component's free function from the interface, and then free the component itself
/// Component should not be used or loaded in the scene after this is called
/// @param component The component to free
inline void component_free(struct component* component){
    if (component->interface->free != nullptr) {
        // Call the provided free function on the component
        component->interface->free(component->data);
    }

    free(component);
}

inline void component_draw_debug_ui(struct component* component){
    if (component->interface->draw_debug_ui != nullptr) {
        component->interface->draw_debug_ui(component->data);
    }
}