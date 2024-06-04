#pragma once
#include "core.h"
#include "drawable.h"
#include "mesh.h"
#include <vector>
#include "cube.h"
#include "constants.h"
#include "skeleton.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

enum class AnimationState
{
	Idle,
	Walking,
	Running,
	Jumping
};

struct AnimationNode
{
	std::string name;
	bool hasBone;
	glm::mat4 transformation;
	glm::mat4 boneTrans;
	int parentIndex;

	std::vector<std::pair<float, glm::vec3>> positions;
	std::vector<std::pair<float, glm::quat>> rotations;
	std::vector<std::pair<float, glm::vec3>> scales;
};

class Model : public Drawable
{
public:
	Model(const std::string &path)
	{
		floorModel = true;
		load_model(path);
	}

	Model(const std::string &modelPath, const std::map<AnimationState, std::string> &animationPath)
	{
		floorModel = false;
		load_model(modelPath);
		loadAnimations(animationPath);
	}

	Model();
	~Model();

	void draw(const glm::mat4 &viewProjMtx, Shader *shader) override;
	void debug_draw(const glm::mat4& viewProjMtx, Shader* shader);
	void loadAnimations(const std::map<AnimationState, std::string> &animationPath);
	void updateAnimations(float deltaTime, AnimationState currentState);
	std::vector<Mesh> meshes;

private:
	float ticks;
	float duration = 0;
	glm::mat4 globalInverse;
	float floorModel = false;

	Skeleton skeleton;
	std::map<AnimationState, std::vector<AnimationNode>> animations;
	// Model data
	
	std::string directory;
	float min_x = FLT_MAX, min_y = FLT_MAX, min_z = FLT_MAX;
	float max_x = FLT_MIN, max_y = FLT_MIN, max_z = FLT_MIN;

	Cube *hitbox;

	void load_model(const std::string &path);

	void process_node(aiNode *node, const aiScene *scene, const glm::mat4 &parentTransform);
	Mesh process_mesh(aiMesh *mesh, const aiScene *scene, const glm::mat4 &transform);

	glm::mat4 interpolatePosition(float time, const AnimationNode &node);
	glm::mat4 interpolateRotation(float time, const AnimationNode &node);
	glm::mat4 interpolateScale(float time, const AnimationNode &node);
	glm::mat4 aiMatrixToGlm(const aiMatrix4x4 &from);
	glm::quat aiQuaternionToGlm(const aiQuaternion &aiQuat);
	glm::vec3 aiVectorToGlm(const aiVector3D &aiVec);

	// Skip textures for now

	// std::vector<Texture> load_mat_textures(aiMaterial* mat, aiTextureType type, std::string typeName);
};