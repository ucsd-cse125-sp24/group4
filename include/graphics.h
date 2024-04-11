#pragma once

/*
 * Graphics.h
 * Includes for the basic graphics client
 * Change OpenGL settings here
 * I don't think any other functions will end up here
 */

#include "shader.h"
#include "window.h"

class Graphics {
public:
	static void setup_opengl_settings() {

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
};