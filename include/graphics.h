#pragma once

/*
 * Graphics.h
 * Includes for the basic graphics client
 * Change OpenGL settings here
 * 
 * Set up window here!
 */

#include "shader.h"
#include "window.h"

class Graphics {
public:
	static void setup_opengl_settings();

	// TODO: Maybe singleton? idk
	static GLFWwindow* set_up_window(int id);
};