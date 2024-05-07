#pragma once
#include "core.h"
#include "Shader.h"

// Abstract class for drawable object

class Drawable
{
protected:
	glm::vec3 color;
	glm::mat4 model;

public:
	virtual void draw(const glm::mat4& viewProjMtx, Shader* shader) = 0;

	// Getters and setters
	glm::mat4 get_world() { return model; }
	void set_color(glm::vec3 c) { color = c; }
	void set_world(glm::mat4 w) { model = w; }

};