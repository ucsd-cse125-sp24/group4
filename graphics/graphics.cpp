#include "../include/graphics.h"
#include "../include/fonts.h"

void Graphics::setup_opengl_settings()
{
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

GLFWwindow *Graphics::set_up_window(int id)
{
	GLFWwindow *window = Window::create_window(800, 600);
	if (!window)
		exit(EXIT_FAILURE);

	Window::player_id = id;
	// Callback functions are registered AFTER creating the window
	// and BEFORE the render loop is initiated
	Graphics::setup_opengl_settings();

	Window::setup_callbacks(window);

	// Set up objects to draw
	Window::setup_scene();

	//----
	// gui::font::FontRenderer fontRenderer;
	// if (!fontRenderer.init())
	// {
	// 	std::cerr << "Failed to initialize font renderer" << std::endl;
	// }

	// char c = 'A';
	// const gui::font::Character &character = fontRenderer.loadChar(c);

	// std::cout << "Character size: " << character.size.x << ", " << character.size.y << std::endl;
	//----

	return window;
}