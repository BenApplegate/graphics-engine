#include "scene.h"

#include <stdlib.h>

struct scene* scene_new(const char* name) {
    struct scene* new_scene = malloc(sizeof(struct scene));
    new_scene->name = name;
    new_scene->objects_list = list_vp_new(10);

    return new_scene;
}

void scene_free(struct scene* scene) {
    list_vp_delete(&scene->objects_list);
    free(scene);
}

void scene_update(struct scene* scene) {

}

void scene_render(struct scene* scene) {

}