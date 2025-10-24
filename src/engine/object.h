#pragma once

#include "../util/list_vp.h"

struct object {
  const char* name; //The name of the object
  struct object* parent; //The object's parent in the scene, nullptr if directly owned by the scene
  struct list_vp children; //List containing this object's children
  struct list_vp components; //List containing this object's components
};

/// Create a new object
/// @param name the name of the object
/// @param parent The parent object of the new object, nullptr if new object has no parent
struct object* object_new(const char* name, struct object* parent);

/// Free object from memory, object should not be loaded in scene after this is called
/// Frees all child objects recursively
/// @param object the object to free
void object_free(struct object* object);

/// Update all components then update all children
/// @param object the object to update
void object_update(struct object* object);