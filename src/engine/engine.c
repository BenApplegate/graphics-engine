#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine.h"

#include "imgui_handler.h"
#include <cimgui/cimgui.h>

struct engine_flags ENGINE_FLAGS = {false};

static struct {
	GLFWwindow* window;
	int window_width;
	int window_height;
} ENGINE_DATA;

/// glfw Framebuffer size callback used for resetting the viewport
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
	ENGINE_DATA.window_width = width;
	ENGINE_DATA.window_height = height;
}

/// Creates the window, OpenGL Context, and loads GLAD
/// @param width The window width
/// @param height The window height
/// @param title The window title
int init_glfw_window(int width, int height, const char *title) {
	if(!glfwInit()){
		fprintf(stderr, "GLFW failed to initialze");
	}
	printf("Initialized GLFW\n");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	ENGINE_DATA.window_width = width;
	ENGINE_DATA.window_height = height;

	ENGINE_DATA.window = glfwCreateWindow(ENGINE_DATA.window_width, ENGINE_DATA.window_height, title, nullptr, nullptr);
	if(ENGINE_DATA.window == nullptr){
		fprintf(stderr, "Failed to create window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(ENGINE_DATA.window);

	glfwSetFramebufferSizeCallback(ENGINE_DATA.window, framebuffer_size_callback);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		fprintf(stderr, "Failed to load GLAD\n");
		glfwTerminate();
		return -2;
	}

	imgui_init();
	return 0;
}

int engine_init(int width, int height, const char* title) {
	if (ENGINE_FLAGS.verbose)
		printf("Graphics engine Starting!\n");

	if (init_glfw_window(width, height, title)) {
		//If the function doesn't return 0 it failed to create an opengl context and we should exit
		return -1;
	};

	glViewport(0, 0, 800, 600);

	glClearColor(0.2f, 0.15f, 0.25f, 1.0f);
	return 0;
}

void engine_render_loop() {
	while(!glfwWindowShouldClose(ENGINE_DATA.window)){
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		imgui_update();

		imgui_render();
		glfwSwapBuffers(ENGINE_DATA.window);


	}
}

void engine_free() {

	imgui_shutdown();
	//Terminate GLFW
	glfwTerminate();
}

GLFWwindow* engine_get_window() {
	return ENGINE_DATA.window;
}