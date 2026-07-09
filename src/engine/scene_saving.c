// Author: Benjamin Applegate

#include "scene.h"

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int scene_save_to_file(const struct scene* scene, const char* filename) {
    //Open file and check that it opened correctly
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file %s\n", filename);
        return 0;
    }

    if (fwrite("BAGE_SCENE", 1, 10, file) != 10) {
        fclose(file);
        fprintf(stderr, "Failed to write file header\n");
        return 0;
    }


    //NOTE: I am using goto's for the error handling in this section as it makes writing the code slightly simpler
    //The endpoint for these gotos can be found at the bottom of the function

    //Write name of scene
    size_t namelen = strlen(scene->name);
    if (fwrite(&namelen, 1, sizeof(size_t), file) != sizeof(size_t)) { goto write_error; }
    //write string
    if (fwrite(scene->name, 1, namelen, file) != namelen) { goto write_error; }

    //Write number of child objects and then write those objects
    if (fwrite(&scene->objects_list.size, sizeof(size_t), 1, file) != 1){goto write_error;}
    for (int i = 0; i < scene->objects_list.size; i++) {
        object_save_to_file(scene->objects_list.data[i], file);
    }

    //Close file and return success
    fclose(file);
    return 1;

    //End point for write error gotos
    write_error:
    fclose(file);
    fprintf(stderr, "Failed to write to file\n");
    return 0;
}

struct scene* scene_load_from_file(const char* filename) {
    if (filename == nullptr) { return nullptr; }

    FILE* file = fopen(filename, "r");
    if (file == nullptr) {
        fprintf(stderr, "Failed to open file %s\n", filename);
        return nullptr;
    }

    //Read header and compare to check that its correct
    char buffer[11];
    if (fread(buffer, 1, 10, file) != 10) {
        fclose(file);
        fprintf(stderr, "File did not have correct file heading, is not a scene file\n");
        return nullptr;
    }
    buffer[10] = 0;
    if (strcmp(buffer, "BAGE_SCENE") != 0) {
        fclose(file);
        fprintf(stderr, "File did not have correct file heading, is not a scene file\n");
        return nullptr;
    }

    //NOTE: I am using goto's for the error handling in this section as it makes writing the code slightly simpler
    //The endpoint for these gotos can be found at the bottom of the function

    //Read name length
    size_t namelen;
    if (fread(&namelen, sizeof(size_t), 1, file) != 1) { goto read_error; }

    //allocate buffer for name, read name, and add null terminator
    char* name_buffer = (char*)malloc(namelen + 1);
    if (fread(name_buffer, 1, namelen, file) != namelen) { goto read_error; }
    name_buffer[namelen] = 0;

    //Create new scene and delete temp buffer
    struct scene* new_scene = scene_new(name_buffer);
    free(name_buffer);

    size_t object_count;
    if (fread(&object_count, sizeof(size_t), 1, file) != 1) { goto read_error; }
    for (int i = 0; i < object_count; i++) {
        struct object* object = object_load_from_file(file, nullptr);
        if (object == nullptr) { scene_free(new_scene); goto read_error; }
        scene_add_object(new_scene, object);
    }

    fclose(file);
    return new_scene;

    //Endpoint for read error goto statements
    read_error:
        fclose(file);
        fprintf(stderr, "There was an error reading the scene file\n");
        return nullptr;
}