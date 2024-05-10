#pragma once
#include "core.h"
#include "drawable.h"
#include <vector>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
};

class Mesh{
public:
	// mesh data
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures; void draw(const glm::mat4 & viewProjMtx, Shader * shader);

		setupMesh();
	}

	void draw(const glm::mat4& viewProjMtx, Shader* shader);

private:
	// Render data
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};