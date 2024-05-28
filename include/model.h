#pragma once
#include "core.h"
#include "drawable.h"
#include "mesh.h"
#include <vector>
#include "cube.h"
#include "constants.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model : public Drawable{
public:
	Model(const std::string& path)
	{
		load_model(path);
	}

	Model();
	~Model();

	void draw(const glm::mat4& viewProjMtx, Shader* shader);

private:
	// Model data
	std::vector<Mesh> meshes;
	std::string directory;
	float min_x = FLT_MAX, min_y = FLT_MAX, min_z = FLT_MAX;
	float max_x = FLT_MIN, max_y = FLT_MIN, max_z = FLT_MIN;

	Cube* hitbox;

	void load_model(const std::string& path);
	void process_node(aiNode* node, const aiScene* scene);
	Mesh process_mesh(aiMesh* mesh, const aiScene* scene);

	// Skip textures for now

	//std::vector<Texture> load_mat_textures(aiMaterial* mat, aiTextureType type, std::string typeName);

};