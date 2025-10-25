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

void component_init(struct component* component);

void component_update(struct component* component);

void component_render(struct component* component);

/// Call the component's free function from the interface, and then free the component itself
/// Component should not be used or loaded in the scene after this is called
/// @param component The component to free
void component_free(struct component* component);

void component_draw_debug_ui(struct component* component);