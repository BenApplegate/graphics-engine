#pragma once

//Engine flags can be changed and accessed data, right now the only option is verbose logging
struct engine_flags {
    int verbose;
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
