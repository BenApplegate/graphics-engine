#pragma once
#include "../util/list_vp.h"

struct scene {
    const char* name;
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
void scene_update(struct scene* scene);

/// Render the given scene, telling all objects and components to render
void scene_render(struct scene* scene);

/// Draws the imgui debug ui for the specified scene
void scene_draw_debug_ui(struct scene* scene);