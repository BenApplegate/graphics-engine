/// Author: Benjamin Applegate

#include <stdlib.h>
#include <glad/glad.h>
#include <math.h>

#include "test_component.h"
#include "cimgui/cimgui.h"
#include "../engine/engine.h"

void init(void* data);
void update(void* data);
void render(void* data);
void delete(void* data);
void draw_debug_ui(void* data);
int save_to_file(void* data, FILE* file);
int load_from_file(void* data, FILE* file);

struct component_interface TEST_COMPONENT_INTERFACE = {
    "Test Component",
    init,
    update,
    render,
    delete,
    draw_debug_ui,
    save_to_file,
    load_from_file
};

void allocate_geometry_data(struct test_component* comp) {
    glBindBuffer(GL_ARRAY_BUFFER, comp->vbo);

    //Create buffer for data and fill buffer with vertex locations for a circle
    size_t buffer_size = (comp->vertex_count + 2) * 2;
    float* vertices = (float*) malloc(buffer_size * sizeof(float));
    comp->vertex_draw_count = comp->vertex_count + 2;
    vertices[0] = comp->offset.x; vertices[1] = comp->offset.y;
    for (int i = 0; i < comp->vertex_count + 1; i++) {
        float ratio = (float) i / (float) comp->vertex_count;
        ratio *= 2.0f * M_PI;

        float x = comp->offset.x + (cosf(ratio) * comp->circleScale) / engine_get_aspect_ratio();
        float y = comp->offset.y + (sinf(ratio) * comp->circleScale);

        vertices[(2 * i) + 2] = x;
        vertices[(2 * i) + 3] = y;
    }

    //Send data to GPU and free CPU buffer
    glBufferData(GL_ARRAY_BUFFER, buffer_size * sizeof(float), vertices, GL_DYNAMIC_DRAW);
    free(vertices);
}

void init(void* data) {
    const auto comp = (struct test_component*) data;

    //Set Initial Data
    comp->circleScale = 0.5f;
    comp->shouldDraw = true;
    comp->vertex_count = 8;
    comp->offset = (ImVec2){0, 0};
    comp->shift_value = 0;
    comp->animate_speed = 0.001f;

    //Create and bind VAO and VBO

    glGenVertexArrays(1, &comp->vao);
    glBindVertexArray(comp->vao);

    glGenBuffers(1, &comp->vbo);
    allocate_geometry_data(comp);

    //Set vertex attributes
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);;
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //Load test shaders
    comp->shader = load_shader("test.vertex", "test.fragment");

    //Init Colors
    for (int i = 0; i < 30; i++) {
        comp->colors[i] = (float) rand() / (float)RAND_MAX;
    }

    //Load uniform locations
    comp->color_uniform = glGetUniformLocation(comp->shader.programID, "colors");
    comp->shift_uniform = glGetUniformLocation(comp->shader.programID, "colorShift");
}

void update(void* data) {
    struct test_component* comp = (struct test_component*) data;

    comp->shift_value += comp->animate_speed;
}

void render(void* data) {
    struct test_component* comp = (struct test_component*) data;
    if (!comp->shouldDraw) return;

    glBindVertexArray(comp->vao);

    //Update uniform values and draw
    glUniform1f(comp->shift_uniform, comp->shift_value);
    glUniform3fv(comp->color_uniform, 10, comp->colors);
    glDrawArrays(GL_TRIANGLE_FAN, 0, comp->vertex_draw_count);
}

void delete(void* data){
    struct test_component* comp = (struct test_component*) data;

    //Delte OpenGL objects then free the component
    glDeleteBuffers(1, &comp->vbo);
    glDeleteVertexArrays(1, &comp->vao);

    free(data);
}

void draw_debug_ui(void* data) {
    struct test_component* comp = (struct test_component*) data;

    igCheckbox("Draw Test Circle", &comp->shouldDraw);

    igSliderInt("Circle Resolution", &comp->vertex_count, 3, 512, nullptr, 0);
    igSliderFloat("Circle Scale", &comp->circleScale, 0.1f, 1.5f, nullptr, 0);
    igSliderFloat2("Circle offset", (float*) &comp->offset, -1.5f, 1.5f, nullptr, 0);

    if (igButton("Regenerate Circle", (ImVec2){0, 0})) {
        allocate_geometry_data(comp);
    }

    igSliderFloat("Animation Speed", &comp->animate_speed, 0.00001f, 0.1f, nullptr, 0);

    if (igCollapsingHeader_BoolPtr("Edit colors", nullptr, 0)) {
        for (int i = 0; i < 10; i++) {
            igPushID_Int(i);
            igText("Color %d", i + 1);
            igColorPicker3("", comp->colors + (3 * i), ImGuiColorEditFlags_DisplayHSV);
            igPopID();
        }
    }

    igSeparatorText("Debug Values");
    igText("VAO: %d\t VBO: %d\nColor Uniform: %d\t Shift Uniform: %d\t Shift Value: %f", comp->vao, comp->vbo, comp->circleScale, comp->color_uniform, comp->shift_uniform, comp->shift_value);
}

struct component* test_component_new() {
    struct component* comp = malloc(sizeof(struct component));
    struct test_component* test_data = malloc(sizeof(struct test_component));
    comp->data = test_data;
    comp->type = TEST;
    comp->interface = &TEST_COMPONENT_INTERFACE;
    return comp;
}

int save_to_file(void* data, FILE* file) {
    struct test_component* comp = (struct test_component*) data;

    if (fwrite(&comp->circleScale, sizeof(float), 1, file) != 1) return -1;
    if (fwrite(&comp->shouldDraw, sizeof(bool), 1, file) != 1) return -1;
    if (fwrite(&comp->vertex_count, sizeof(int), 1, file) != 1) return -1;
    if (fwrite(&comp->offset, sizeof(ImVec2), 1, file) != 1) return -1;
    if (fwrite(&comp->shift_value, sizeof(float), 1, file) != 1) return -1;
    if (fwrite(&comp->animate_speed, sizeof(float), 1, file) != 1) return -1;
    if (fwrite(comp->colors, sizeof(float), 30, file) != 30) return -1;

    return 0;
}

int load_from_file(void* data, FILE* file) {
    struct test_component* comp = (struct test_component*) data;

    if (fread(&comp->circleScale, sizeof(float), 1, file) != 1) return -1;
    if (fread(&comp->shouldDraw, sizeof(bool), 1, file) != 1) return -1;
    if (fread(&comp->vertex_count, sizeof(int), 1, file) != 1) return -1;
    if (fread(&comp->offset, sizeof(ImVec2), 1, file) != 1) return -1;
    if (fread(&comp->shift_value, sizeof(float), 1, file) != 1) return -1;
    if (fread(&comp->animate_speed, sizeof(float), 1, file) != 1) return -1;
    if (fread(comp->colors, sizeof(float), 30, file) != 30) return -1;

    //Create and bind VAO and VBO

    glGenVertexArrays(1, &comp->vao);
    glBindVertexArray(comp->vao);

    glGenBuffers(1, &comp->vbo);
    allocate_geometry_data(comp);

    //Set vertex attributes
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);;
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //Load test shaders
    comp->shader = load_shader("test.vertex", "test.fragment");

    //Load uniform locations
    comp->color_uniform = glGetUniformLocation(comp->shader.programID, "colors");
    comp->shift_uniform = glGetUniformLocation(comp->shader.programID, "colorShift");

    return 0;
}