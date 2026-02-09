/// Author: Benjamin Applegate

#include "scene.h"

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cimgui/cimgui.h>

#include "object.h"

struct scene* scene_new(const char* name) {
    struct scene* new_scene = malloc(sizeof(struct scene));
    new_scene->name = strdup(name);
    new_scene->objects_list = list_vp_new(10);

    return new_scene;
}

void scene_free(struct scene* scene) {
    //Free all scene objects
    for (int i = 0; i < scene->objects_list.size; i++) {
        object_free(scene->objects_list.data[i]);
    }

    //Free scene data
    list_vp_delete(&scene->objects_list);
    free(scene->name);
    free(scene);
}

void scene_update(const struct scene* scene) {
    for (int i = 0; i < scene->objects_list.size; i++) {
        object_update(list_vp_at(&scene->objects_list, i));
    }
}

void scene_render(const struct scene* scene) {
    for (int i = 0; i < scene->objects_list.size; i++) {
        object_render(list_vp_at(&scene->objects_list, i));
    }
}

void scene_add_new_object(struct scene* scene, const char* name) {
    list_vp_add(&scene->objects_list, object_new(name, nullptr));
}

void scene_add_object(struct scene* scene, struct object* object) {
    list_vp_add(&scene->objects_list, object);
}

void scene_draw_debug_ui(struct scene* scene) {
    //Construct name for window
    //TODO: Dont use hardcoded length for window name, instead calculate needed length
    char window_name[256];
    strcpy(window_name, "Scene: ");
    strcat(window_name, scene->name);
    igBegin(window_name, nullptr, ImGuiWindowFlags_None);

    //Display number of objects in scene
    igText("Objects: %i", scene->objects_list.size);
    igText("Current Objects Capacity: %i", scene->objects_list.capacity);

    igSeparator();

    //Add popup to add scene item
    ImVec2 button_size = {0, 0};
    if (igButton("Add Object", button_size)) {
        igOpenPopup_Str("Add Scene Object", ImGuiPopupFlags_None);
    }

    ImVec2 center;
    ImGuiViewport_GetCenter(&center, igGetMainViewport());
    igSetNextWindowPos(center, ImGuiCond_Appearing, (ImVec2){0.5f, 0.5f});

    if (igBeginPopupModal("Add Scene Object", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        static char object_name[32] = "New Object";

        igInputText("Object Name", object_name, 32, ImGuiInputTextFlags_None, nullptr, nullptr);

        //Cancel and close button
        if (igButton("Add Object", button_size)) {
            scene_add_new_object(scene, object_name);
            igCloseCurrentPopup();
        }
        igSameLine(0, 20);
        if (igButton("Close", button_size)) { igCloseCurrentPopup(); }

        igEndPopup();
    }

    //Draw Scene Tree
    if (igTreeNodeEx_Str("Scene Tree", ImGuiTreeNodeFlags_DrawLinesFull)) {
        for (int i = 0; i < scene->objects_list.size; i++) {
            igPushID_Int(i);
            object_draw_scene_tree_node(scene->objects_list.data[i]);
            igPopID();
        }

        igTreePop();
    }
    igEnd();

    //Now we draw windows for any objects inside the scene
    for (int i = 0; i < scene->objects_list.size; i++) {
        igPushID_Int(i);
        object_draw_debug_window(scene->objects_list.data[i]);
        igPopID();
    }
}

struct scene* scene_create_test_scene() {
    struct scene* scene = scene_new("Test Scene");

    struct object* test_object = object_new("Test Object", nullptr);
    object_add_new_component_of_type(test_object, TEST);

    scene_add_object(scene, test_object);
    return scene;
}

int scene_save_to_file(const struct scene* scene, const char* filename) {
    //First we create the file to save the information to
    int file_descriptor = creat(filename, 644);
    if (file_descriptor == -1) {
        return 0;
    }

    //Write file header to mark filetype and return if write fails
    if (write(file_descriptor, "BAGE_SCENE", 10) != 10) {
        close(file_descriptor);
        return 0;
    }

    //Write name of scene
    size_t namelen = strlen(scene->name);

    //NOTE: I am using goto's for the error handling in this section as it makes writing the code slightly simpler
    //The endpoint for these gotos can be found at the bottom of the function

    //write length of string
    if (write(file_descriptor, &namelen, sizeof(size_t)) != sizeof(size_t)) { goto write_error; }
    //write string
    if (write(file_descriptor, scene->name, namelen) != namelen) { goto write_error; }

    //Close file and return success
    close(file_descriptor);
    return 1;

    //End point for write error gotos
    write_error:
    close(file_descriptor);
    return 0;
}

struct scene* scene_load_from_file(const char* filename) {
    if (filename == nullptr) { return nullptr; }

    int file_descriptor = open(filename, O_RDONLY);
    if (file_descriptor == -1) {
        fprintf(stderr, "Failed to open file %s\n", filename);
        return nullptr;
    }

    char buffer[11];
    if (read(file_descriptor, buffer, 10) != 10) {
        close(file_descriptor);
        return nullptr;
    }

    //Check for correct file heading
    if (strcmp(buffer, "BAGE_SCENE") != 0) {
        close(file_descriptor);
        fprintf(stderr, "File did not have correct file heading, is not a scene file\n");
        return nullptr;
    }

    //NOTE: I am using goto's for the error handling in this section as it makes writing the code slightly simpler
    //The endpoint for these gotos can be found at the bottom of the function

    //Read name length
    size_t namelen;
    if (read(file_descriptor, &namelen, sizeof(size_t)) != sizeof(size_t)) { goto read_error; }

    //allocate buffer for name, read name, and add null terminator
    char* name_buffer = (char*)malloc(namelen + 1);
    if (read(file_descriptor, name_buffer, namelen) != namelen) { goto read_error; }
    name_buffer[namelen] = 0;

    //Create new scene and delete temp buffer
    struct scene* new_scene = scene_new(name_buffer);
    free(name_buffer);

    close(file_descriptor);
    return new_scene;

    //Endpoint for read error goto statements
    read_error:
        close(file_descriptor);
        fprintf(stderr, "File did not have correct file heading, is not a scene file\n");
        return nullptr;
}