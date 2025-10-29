/// Author: Benjamin Applegate
/// Defines the main functions of the engine and exposes a global set of flags for the engine to use
/// Handles window creation, scene loading, the update-render loop, and engine shutdown

#pragma once
#include <GLFW/glfw3.h>

//Engine flags can be changed and accessed data, right now the only option is verbose logging
struct engine_flags {
    int verbose;
    int show_imgui_windows; //0 = no imgui at all, 1 = show imgui, 2 = draw test windows
};

extern struct engine_flags ENGINE_FLAGS;

/// Initializes the engine with the given window size and title
/// @param width The width of the window
/// @param height The height of the window
/// @param title The title of the window
int engine_init(int width, int height, const char* title);

/// Starts the main render loop of the engine, all data should be setup before calling this
/// Blocks until the application window closes
void engine_render_loop();

/// Free all engine resources
/// attempts to dispose of all currently loaded engine data
void engine_free();

/// @returns A pointer to the engine's GLFWwindow
GLFWwindow* engine_get_window();

/// Creates a new scene and loads it into the engine
/// @param name The name of the newly created scene
void engine_load_new_scene(const char* name);

/// @returns A pointer to the list of scenes loaded in the engine
struct list_vp* engine_get_scenes();

/// @returns The current aspect ratio of the engine's window as a float
float engine_get_aspect_ratio();