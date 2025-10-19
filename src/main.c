#include <stdio.h>
#include "engine/engine.h"

int main(void) {
    //Enable verbose logging
    ENGINE_FLAGS.verbose = true;

    engine_init(800, 600, "Cool engine test");

    engine_render_loop();

    engine_free();
}

