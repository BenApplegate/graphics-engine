/// Author: Benjamin Applegate
/// Implements a scene object that can hold child objects and components

#pragma once

#include "../util/list_vp.h"
#include "component.h"

struct object {
  char* name; //The name of the object
  struct object* parent; //The object's parent in the scene, nullptr if directly owned by the scene
  struct list_vp children; //List containing this object's children
  struct list_vp components; //List containing this object's components

  bool show_debug_window; // Decides if the debug window should be rendered
};

/// Create a new object
/// @warning Heap allocates the object, remember to call object_free when this is no longer being used
/// @param name the name of the object
/// @param parent The parent object of the new object, nullptr if new object has no parent
struct object* object_new(const char* name, struct object* parent);

/// Free object from memory, object should not be loaded in scene after this is called
/// Frees all child objects recursively
/// @param object the object to free
void object_free(struct object* object);

/// Update all children then update components
/// @param object the object to update
void object_update(const struct object* object);

/// Render all children then render components
/// @param object the object to render
void object_render(const struct object* object);

/// Create a new object and set it as a child of the given object
/// @param object the object to create a new child for
/// @param child_name the name of the new child
void object_add_new_child(struct object* object, const char* child_name);

/// Draw a tree node for this object in the scene tree window
/// @param object the object to draw the tree node for
void object_draw_scene_tree_node(struct object* object);

/// Draws the imgui debug window for the specified object and its children
/// @param object The object to draw the window for
void object_draw_debug_window(struct object* object);

/// Adds a new component to the object of the specified type
/// @param object The object to attach a new component to
/// @param type The type of component to add
void object_add_new_component_of_type(struct object* object, enum COMPONENT_TYPE type);