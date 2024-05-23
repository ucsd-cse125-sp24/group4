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
	std::vector<std::pair<float, glm::vec3>> positions;
	std::vector<std::pair<float, glm::quat>> rotations;
	std::vector<std::pair<float, glm::vec3>> scales;
};

struct Animation
{
	std::string name;
	float duration;
	float ticksPerSecond;
	std::map<std::string, AnimationNode> channels;
};

class Model : public Drawable
{
public:
	Model(const std::string &path)
	{
		load_model(path);
	}

	Model(const std::string &modelPath, const std::map<AnimationState, std::string> &animationPath)
	{
		load_model(modelPath);
		loadAnimations(animationPath);
	}

	Model();
	~Model();

	void draw(const glm::mat4 &viewProjMtx, Shader *shader);
	void loadAnimations(const std::map<AnimationState, std::string> &animationPath);
	void updateAnimations(float deltaTime, AnimationState currentState);

private:
	Skeleton skeleton;
	std::map<AnimationState, Animation> animations;
	// Model data
	std::vector<Mesh> meshes;
	std::string directory;
	float min_x = INT_MAX, min_y = INT_MAX, min_z = INT_MAX;
	float max_x = INT_MIN, max_y = INT_MIN, max_z = INT_MIN;

	Cube *hitbox;

	void load_model(const std::string &path);
	
	void loadAnimationFromPath(AnimationState state, const std::string &path);
	glm::mat4 aiMatrixToGlm(const aiMatrix4x4 &from);
	glm::quat aiQuaternionToGlm(const aiQuaternion &aiQuat);
	glm::vec3 aiVectorToGlm(const aiVector3D &aiVec);
	void process_node(aiNode *node, const aiScene *scene);
	Mesh process_mesh(aiMesh *mesh, const aiScene *scene);

	glm::vec3 interpolatePosition(float time, const AnimationNode &node);

	glm::quat interpolateRotation(float time, const AnimationNode &node);

	glm::vec3 interpolateScale(float time, const AnimationNode &node);

	

	// Skip textures for now

	// std::vector<Texture> load_mat_textures(aiMaterial* mat, aiTextureType type, std::string typeName);
};