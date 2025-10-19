#include <stdio.h>
#include "engine/engine.h"

int main(void) {
    //Enable verbose logging
    ENGINE_FLAGS.verbose = true;

    start_engine(800, 600, "Cool engine test");
}

