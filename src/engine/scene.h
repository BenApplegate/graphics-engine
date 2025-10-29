/// Author: Benjamin Applegate

#pragma once
#include "../util/list_vp.h"

struct scene {
    char* name;
    struct list_vp objects_list;
};

/// Initialize a new empty scene
/// @param name The name of the scene
struct scene* scene_new(const char* name);

/// Cleanup all scene resources
/// Scene should not be used or loaded in engine after this is called
/// @param scene The scene to free
void scene_free(struct scene* scene);

/// Update given scene, updating all objects and underlying components
/// @param scene The scene to update
void scene_update(const struct scene* scene);

/// Render the given scene, telling all objects and components to render
/// @param scene The scene to render
void scene_render(const struct scene* scene);

/// Add a new child object to the scene with the given name
/// @param scene The scene to create a new object in
/// @param name The name to give the newly created object
void scene_add_new_object(struct scene* scene, const char* name);

/// Draws the imgui debug ui for the specified scene
/// @param scene The scene to draw the UI for
void scene_draw_debug_ui(struct scene* scene);