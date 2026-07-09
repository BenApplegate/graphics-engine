/// Author: Benjamin Applegate
/// Defines an interface for components in the engine

#pragma once

#include <stdio.h>

/// Holds the type of the component, can be used to identify data type of contained info
enum COMPONENT_TYPE {
    TEST
};

///Provides an interface for the engine to interact with components
struct component_interface {
    const char* printable_name;
    void (*init)(void* data); //This function is called when the component is loaded into the object
    void (*update)(void* data); //This function is called every frame
    void (*render)(void* data); //This function is called every frame after update has ran for the entire scene
    void (*free)(void* data); //This function is called to delete component data
    void (*draw_debug_ui)(void* data); //This function is called to draw this component's imgui UI
    int (*save_to_file)(void* data, FILE* file); //This function is called whenever the engine is saving the scene
    int (*load_from_file)(void* data, FILE* file); //This function will load the component data from a file
};

struct component {
    enum COMPONENT_TYPE type;
    void* data;
    const struct component_interface* interface;
};

/// Creates a new instance of the requested component type
/// @param type The type of component to create
struct component* get_new_component_of_type(enum COMPONENT_TYPE type);

///Calls the underlying init function on the provided component if it exists
///@param component The component to initialize
void component_init(struct component* component);

///Calls the underlying update function on the provided component if it exists
///@param component The component to update
void component_update(struct component* component);

///Calls the underlying render function on the provided component if it exists
///@param component The component to render
void component_render(struct component* component);

///Calls the underlying free function on the provided component if it exists
///@warning The component should not be attached to an object after this is called
///@param component The component to free
void component_free(struct component* component);

///Calls the underlying draw_debug_ui function on the provided component if it exists
///@param component The component to draw UI for
void component_draw_debug_ui(struct component* component);

/// Saves the specified component to a file
/// @param component the component to save to the file
/// @param file the file to save the component to
/// @returns 0 if success, nonzero on error
int component_save_to_file(struct component* component, FILE* file);

/// Loads a component from a given file
/// @param file the file to load the component from
/// @returns the component
struct component* component_load_from_file(FILE* file);