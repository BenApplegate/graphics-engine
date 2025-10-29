/// Author: Benjamin Applegate
/// Contains the entry point of the engine, setting flags and information before starting the engine

#include "engine/engine.h"

int main(void) {
    ENGINE_FLAGS.verbose = true;
    ENGINE_FLAGS.show_imgui_windows = 2;

    engine_init(1920, 1080, "Cool engine test");

    engine_render_loop();

    engine_free();
}

