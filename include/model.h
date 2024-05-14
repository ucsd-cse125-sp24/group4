#pragma once
#include "core.h"
#include "drawable.h"
#include "mesh.h"
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model : public Drawable{
public:
	Model(char* path)
	{
		load_model(path);
	}

	Model();

	void draw(const glm::mat4& viewProjMtx, Shader* shader);

	Model* clone();

private:
	// Model data
	std::vector<Mesh> meshes;
	std::string directory;

	void load_model(std::string path);
	void process_node(aiNode* node, const aiScene* scene);
	Mesh process_mesh(aiMesh* mesh, const aiScene* scene);

	// Skip textures for now

	//std::vector<Texture> load_mat_textures(aiMaterial* mat, aiTextureType type, std::string typeName);

};