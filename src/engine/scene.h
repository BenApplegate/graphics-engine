#pragma once
#include "../util/list_vp.h"

struct scene {
    const char* name;
    struct list_vp objects;
};

/// Initialize a new empty scene
/// @param name The name of the scene
void scene_new(const char* name);

/// Cleanup all scene resources
/// Scene should not be used or loaded in engine after this is called
/// @param scene The scene to free
void scene_free(struct scene* scene);
