#pragma once
#include "core.h"
#include "shader.h"
#include <vector>

class Cube {
private:
	GLuint VAO;
	GLuint VBO_positions, VBO_normals, EBO;
	
	glm::mat4 model; // Possibly decouple from cube/primitives in general
					 // if we want to render multiple of the same object
	glm::vec3 color;

	// cube information
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

public:
	Cube(glm::vec3 cubeMin = glm::vec3(-1, -1, -1), glm::vec3 cubeMax = glm::vec3(1, 1, 1));
	~Cube();

	void draw(const glm::mat4& viewProjMtx, Shader* shader);
	void move(glm::vec3 direction);

	// Getters and setters
	glm::mat4 get_world() { return model; }
	void set_color(glm::vec3 color) { this->color = color; }
};