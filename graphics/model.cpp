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

	// Draw the hitbox as a wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glm::mat4 t = glm::mat4(1.0f);
	//t[3] = model[3];
	//hitbox->set_world(t);
	//float scale = 1 / PLAYER_MODEL_SCALE;
	//glm::mat4 t = glm::scale(model, glm::vec3(scale, scale, scale));
	//hitbox->set_world(t);
	hitbox->draw(viewProjMtx, shader);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	glUseProgram(0);
}

void Model::load_model(const std::string& path) {
	Assimp::Importer importer;
	
	// Get current directory in Windows
	// char buffer[MAX_PATH];
	// GetCurrentDirectory(MAX_PATH, buffer);
	// std::string current_dir(buffer);
	// std::cout << "Current directory: " << current_dir << std::endl;

	unsigned int processFlags =
		aiProcess_CalcTangentSpace | // calculate tangents and bitangents if possible
		aiProcess_JoinIdenticalVertices | // join identical vertices/ optimize indexing
		//aiProcess_ValidateDataStructure  | // perform a full validation of the loader's output
		aiProcess_Triangulate | // Ensure all verticies are triangulated (each 3 vertices are triangle)
		aiProcess_ConvertToLeftHanded | // convert everything to D3D left handed space (by default right-handed, for OpenGL)
		aiProcess_SortByPType | // ?
		aiProcess_ImproveCacheLocality | // improve the cache locality of the output vertices
		aiProcess_RemoveRedundantMaterials | // remove redundant materials
		aiProcess_FindDegenerates | // remove degenerated polygons from the import
		aiProcess_FindInvalidData | // detect invalid model data, such as invalid normal vectors
		aiProcess_GenUVCoords | // convert spherical, cylindrical, box and planar mapping to proper UVs
		aiProcess_TransformUVCoords | // preprocess UV transformations (scaling, translation ...)
		aiProcess_FindInstances | // search for instanced meshes and remove them by references to one master
		aiProcess_LimitBoneWeights | // limit bone weights to 4 per vertex
		aiProcess_OptimizeMeshes | // join small meshes, if possible;
		aiProcess_PreTransformVertices | //-- fixes the transformation issue.
		aiProcess_SplitByBoneCount | // split meshes with too many bones. Necessary for our (limited) hardware skinning shader
		aiProcess_GenBoundingBoxes | // generate bounding boxes from meshes
		0;

	//const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	const aiScene* scene = importer.ReadFile(path, processFlags);

	/*
	 * Useful options: aiProcess_GenNormals, aiProcess_SplitLargeMeshes, aiProcess_OptimizeMeshes
	 */

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	process_node(scene->mRootNode, scene);

	// Create a hitbox for the model
	hitbox = new Cube(glm::vec3(min_x, min_y, min_z), glm::vec3(max_x, max_y, max_z));
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

	min_x = ((min_x) < (mesh->mAABB.mMin.x)) ? (min_x) : (mesh->mAABB.mMin.x);
	min_y = ((min_y) < (mesh->mAABB.mMin.y)) ? (min_y) : (mesh->mAABB.mMin.y);
	min_z = ((min_z) < (mesh->mAABB.mMin.z)) ? (min_z) : (mesh->mAABB.mMin.z);

	max_x = ((max_x) > (mesh->mAABB.mMax.x)) ? (max_x) : (mesh->mAABB.mMax.x);
	max_y = ((max_y) > (mesh->mAABB.mMax.y)) ? (max_y) : (mesh->mAABB.mMax.y);
	max_z = ((max_z) > (mesh->mAABB.mMax.z)) ? (max_z) : (mesh->mAABB.mMax.z);
	

	return Mesh(vertices, indices, textures);
}

Model::~Model() {
	delete hitbox;
}