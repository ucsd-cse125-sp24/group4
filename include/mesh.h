#pragma once
#include "core.h"
#include "drawable.h"
#include <vector>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	// glm::ivec4 boneIndices;
	// glm::vec4 boneWeights;
};

struct Texture
{
	unsigned int id;
	std::string type;
};

struct BoneData
{
	glm::ivec4 boneIndices;
	glm::vec4 boneWeights;

	BoneData() : boneIndices(0), boneWeights(0.0f) {}

	void addBoneData(int boneIndex, float weight)
	{
		for (int i = 0; i < 4; ++i)
		{
			if (boneWeights[i] == 0.0f)
			{
				boneIndices[i] = boneIndex;
				boneWeights[i] = weight;
				return;
			}
		}
	}
};

class Mesh
{
public:
	// mesh data
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	std::vector<BoneData> bones;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, std::vector<BoneData> bones)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		this->bones = bones;
		void draw(const glm::mat4 &viewProjMtx, Shader *shader);

		setupMesh();
	}

	void draw(const glm::mat4 &viewProjMtx, Shader *shader);

	// void addBoneData(unsigned int vertexId, int boneID, float weight) {
	//     if (vertexId >= vertices.size()) {
	//         std::cerr << "Vertex index out of range." << std::endl;
	//         return;
	//     }

	//     Vertex& vertex = vertices[vertexId];
	//     bool added = false;
	//     for (int i = 0; i < 4; ++i) {
	//         if (vertex.boneWeights[i] == 0.0f) {
	//             vertex.boneIndices[i] = boneID;
	//             vertex.boneWeights[i] = weight;
	//             added = true;
	//             break;
	//         }
	//     }
	//     if (!added) {
	//         std::cerr << "Vertex has maximum bones already." << std::endl;
	//     }
	// }

private:
	// Render data
	unsigned int VAO, VBO, EBO, boneVBO;
	void setupMesh();
};