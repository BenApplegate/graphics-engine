#pragma once

#include "../engine/component.h"
#include "../engine/shader.h"
#include "cimgui/cimgui.h"

extern struct component_interface TEST_COMPONENT_INTERFACE;

struct test_component {
    unsigned int vao;
    unsigned int vbo;
    int vertex_count;
    int vertex_draw_count;
    float circleScale;
    bool shouldDraw;
    ImVec2 offset;
    float colors[30];
    int color_uniform;
    int shift_uniform;
    float shift_value;
    float animate_speed;

    struct shader shader;
};

struct component* test_component_new();