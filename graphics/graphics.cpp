#include "../include/graphics.h"

void Graphics::setup_opengl_settings() {
	// TODO: Understand this :sob:
	// Enable Depth Testing
	glEnable(GL_DEPTH_TEST);
	// Related to shaders and z value comparisons for the depth buffer.
	glDepthFunc(GL_LEQUAL);

	// Polygon drawing mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Set clear color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

GLFWwindow* Graphics::set_up_window() {
	GLFWwindow* window = Window::create_window(800, 600);
	if (!window) exit(EXIT_FAILURE);

	// Callback functions are registered AFTER creating the window
	// and BEFORE the render loop is initiated
	Graphics::setup_opengl_settings();

	Window::setup_callbacks(window);

	// Set up objects to draw
	Window::setup_scene();

	return window;
}