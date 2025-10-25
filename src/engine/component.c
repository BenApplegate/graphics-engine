#include "component.h"

#include <stdio.h>
#include <stdlib.h>

#include "engine.h"
#include "../components/test_component.h"

struct component* get_new_component_of_type(enum COMPONENT_TYPE type) {
    switch (type) {
        case TEST:
            return test_component_new();
            break;
        default:
            if (ENGINE_FLAGS.verbose) fprintf(stderr, "WARNING: could not load component type: %i\n", type);
            return nullptr;
    }
}