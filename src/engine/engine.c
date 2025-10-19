#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine.h"

#include "mesh.h"
#include "shader.h"

struct engine_flags ENGINE_FLAGS = {false};

static struct engine_data {
	GLFWwindow* window;
	int window_width;
	int window_height;
} ENGINE_DATA;

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
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

	ENGINE_DATA.window = glfwCreateWindow(ENGINE_DATA.window_width, ENGINE_DATA.window_height, title, NULL, NULL);
	if(ENGINE_DATA.window == NULL){
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

	return 0;
}

int start_engine(int width, int height, const char* title) {
	if (ENGINE_FLAGS.verbose)
		printf("Graphics engine Starting!\n");

	bool returns;
	if (init_glfw_window(width, height, title)) {
		//If the function doesnt return 0 it failed to create an opengl context and we should exit
		return -1;
	};

	glViewport(0, 0, 800, 600);

	glClearColor(0.2, 0.15, 0.25, 1.0);

	struct mesh test_mesh = setup_test_triangle();
	struct shader shader2 = load_shader("test.vertex", "test.fragment");

	//Render loop
	while(!glfwWindowShouldClose(ENGINE_DATA.window)){

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(ENGINE_DATA.window);
		glfwPollEvents();

	}

	//Free resources
	glfwTerminate();
	return 0;
}
