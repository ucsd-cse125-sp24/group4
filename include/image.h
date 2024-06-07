#pragma once
#include "core.h"
#include "stb_image.h"
#include <vector>
#include "shader.h"

class Image {
private:
	GLuint VAO, VBO, EBO;
	GLuint texture;
	// Do I want a color?
	// If yes, send it as uniform to fragment shader
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

public:
	Image(const std::string& path);
	void draw(Shader* shader);

};