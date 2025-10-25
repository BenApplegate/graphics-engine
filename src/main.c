#include <stdio.h>
#include "engine/engine.h"

int main(void) {
    //Enable verbose logging
    ENGINE_FLAGS.verbose = true;
    ENGINE_FLAGS.show_imgui_windows = 2;

    engine_init(1920, 1080, "Cool engine test");

    engine_render_loop();

    engine_free();
}

