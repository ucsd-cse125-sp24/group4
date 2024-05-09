#include "../include/model.h"

#include <iostream> // For std::cout
#include <string>   // For std::string
#include <Windows.h> // For GetCurrentDirectory and MAX_PATH

void Model::draw(const glm::mat4& viewProjMtx, Shader* shader) {

	shader->activate();
	shader->set_mat4("model", (float*)&model);
	shader->set_mat4("viewProj", (float*)&viewProjMtx);
	shader->set_vec3("DiffuseColor", &color[0]);

	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].draw(viewProjMtx, shader);
	}

	glUseProgram(0);
}

void Model::load_model(std::string path) {
	Assimp::Importer importer;
	
	// Get current directory in Windows
	// char buffer[MAX_PATH];
	// GetCurrentDirectory(MAX_PATH, buffer);
	// std::string current_dir(buffer);
	// std::cout << "Current directory: " << current_dir << std::endl;

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	/*
	 * Useful options: aiProcess_GenNormals, aiProcess_SplitLargeMeshes, aiProcess_OptimizeMeshes
	 */

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	process_node(scene->mRootNode, scene);
}

void Model::process_node(aiNode* node, const aiScene* scene) {

	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(process_mesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		process_node(node->mChildren[i], scene);
	}
}

Mesh Model::process_mesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		// Process vertex positions, normals and texture coordinates
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;

		// Skip the textures for now
		//if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		//{
		//	glm::vec2 vec;
		//	vec.x = mesh->mTextureCoords[0][i].x;
		//	vec.y = mesh->mTextureCoords[0][i].y;
		//	vertex.texCoords = vec;
		//}
		//else
			vertex.texCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	// Process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// Process material - used for textures...

	return Mesh(vertices, indices, textures);
}


