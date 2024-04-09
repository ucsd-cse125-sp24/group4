#include "window.h"

int Window::width;
int Window::height;
const char* Window::window_title = "Graphics Client";
Shader* Window::shader_program = nullptr;

// TODO: Replace with Scene later
Cube* Window::cube = nullptr;


GLFWwindow* Window::create_window(int width, int height) {
	Window::width = width;
	Window::height = height;
	
	// Initialize GLFW
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// For Mac OS X:
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// 4x antialiasing?
	// glfwWindowHint(GLFW_SAMPLES, 4);

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window) {
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(window);

	// Call AFTER making window and making it current
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return NULL;
	}

	glViewport(0, 0, 800, 600);


	// Shader program - maybe move somewhere else?
	// Initialize shader
	shader_program = new Shader("shaders/shader.vert", "shaders/shader.frag");

	// TODO: Set up camera here

	// TODO: Initialize the interaction variables?

	// Call the resize callback to make sure things get drawn immediately.
	Window::resize_callback(window, width, height);

	return window;
}

void Window::setup_callbacks(GLFWwindow* window) {
	// Set the required callback functions

	/* glfwSetCursorPosCallback(window, Window::mouse_callback);
	glfwSetScrollCallback(window, Window::scroll_callback);*/

	glfwSetKeyCallback(window, Window::key_callback);
	glfwSetWindowSizeCallback(window, Window::resize_callback);
}

void Window::setup_scene() {
	cube = new Cube();
}

void Window::clean_up() {
	// Deallcoate the objects
	delete cube;

	// Delete the shader program
	delete shader_program;

}

// CALLBACKS -------------------------------------------------------------------
void Window::resize_callback(GLFWwindow* window, int width, int height) {
	Window::width = width;
	Window::height = height;
	// Set the viewport size
	glViewport(0, 0, width, height);

	// TODO Camera re-set aspect ratio
	//    Cam->SetAspect(float(width) / float(height));
}

void Window::display_callback(GLFWwindow* window) {
	// Main rendering loop
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// TODO: Render any objects you need to here
	// TODO - render cube. BUT I NEED A CAMERA FIRST!

	// TODO - hack a viewprojection matrix for now
	glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f, 10.0f, 10.0f),  // Camera position
		glm::vec3(0.0f, 0.0f, 0.0f),  // Look at position
		glm::vec3(0.0f, 1.0f, 0.0f)); // Up vector

	float aspectRatio = width / height; // Adjust this according to your screen size
	float fov = glm::radians(45.0f); // Field of view in degrees

	glm::mat4 projectionMatrix = glm::perspective(fov, aspectRatio, 0.1f, 100.0f); // Perspective projection

	// View-Projection matrix
	glm::mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;

	cube->draw(viewProjectionMatrix, shader_program);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();

	// Swap buffers
	glfwSwapBuffers(window);

}

void Window::idle_callback() {
	// Perform any updates as necessary
	// This is called every frame
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// Check for a keypress
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_W:
			// Forward
			// TODO: Abstract into EVENTS for the server
			cube->move(glm::vec3(0, 0, -1));
			break;
		case GLFW_KEY_S:
			// Back
			cube->move(glm::vec3(0, 0, 1));
			break;
		case GLFW_KEY_A:
			// Left
			cube->move(glm::vec3(-1, 0, 0));
			break;
		case GLFW_KEY_D:
			// Right
			cube->move(glm::vec3(1, 0, 0));
			break;

		// TODO: Reset camera? once we have one


		}
	}
}