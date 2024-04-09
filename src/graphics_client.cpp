#include "core.h"
#include "graphics.h"

int main()
{
	GLFWwindow* window = Window::create_window(800, 600);
	if (!window) exit(EXIT_FAILURE);

	// Callback functions are registered AFTER creating the window
	// and BEFORE the render loop is initiated
	Graphics::setup_opengl_settings();
	
	Window::setup_callbacks(window);

	// Set up objects to draw
	Window::setup_scene();

	// render loop
	while (!glfwWindowShouldClose(window)) {

		Window::display_callback(window);

		// TODO: Updates (idle callback)

	}

	Window::clean_up();

	glfwTerminate();
	return 0;
}