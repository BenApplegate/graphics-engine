#include "imgui_handler.h"

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui/cimgui.h>
#define CIMGUI_USE_OPENGL3
#define CIMGUI_USE_GLFW
#include <cimgui/cimgui_impl.h>

#include "engine.h"

static ImGuiContext* imgui_context;
static ImGuiIO* imgui_io;

void imgui_init() {
    imgui_context = igCreateContext(nullptr);
    imgui_io = igGetIO_Nil();

    const char* glsl_version = "#version 330 core";
    ImGui_ImplGlfw_InitForOpenGL(engine_get_window(), true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    igStyleColorsDark(nullptr);
    igGetStyle()->FontScaleMain *= 2.0f;
    ImGuiStyle_ScaleAllSizes(igGetStyle(), 3.0f);

}

void imgui_shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    igDestroyContext(imgui_context);
}

void imgui_update() {
    if (ENGINE_FLAGS.show_imgui_windows < 1) return;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    igNewFrame();

    if (ENGINE_FLAGS.show_imgui_windows >= 2) {
        igShowDemoWindow(nullptr);

        igBegin("Test window", nullptr, 0);
        igText("If you see this, the thing is working");
        igEnd();
    }
}

void imgui_render() {
    if(ENGINE_FLAGS.show_imgui_windows < 1) return;

    igRender();
    ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
}