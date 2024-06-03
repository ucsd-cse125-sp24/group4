#include "../include/model.h"

#include <iostream>  // For std::cout
#include <string>    // For std::string
#include <Windows.h> // For GetCurrentDirectory and MAX_PATH
#include "model.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "../include/glm/gtx/string_cast.hpp"
#include <algorithm>
#include <fstream>

#ifdef min
#undef min
#endif
#include <stack>
#ifdef max
#undef max
#endif
void writeBoundingBoxToTextFile(const glm::vec3& minVec, const glm::vec3& maxVec) {
    std::ofstream file("stat", std::ios::app); // Open in text mode to append data
    if (!file) {
        std::cerr << "Failed to open the file for writing.\n";
        return;
    }

    file << minVec.x << ", " << minVec.y << ", " << minVec.z << std::endl;
    file << maxVec.x << ", " << maxVec.y << ", " << maxVec.z << "." << std::endl;
    file.close();
}
void Model::draw(const glm::mat4 &viewProjMtx, Shader *shader)
{

    shader->activate();
    shader->set_mat4("model", (float *)&model);
    shader->set_mat4("viewProj", (float *)&viewProjMtx);
    shader->set_vec3("DiffuseColor", &color[0]);

    std::vector<glm::mat4> boneMatrices(skeleton.bones.size());

    for (size_t i = 0; i < skeleton.bones.size(); ++i)
    {
        boneMatrices[i] = skeleton.bones[i].finalTransformation;
    }
    shader->set_mat4_array("boneMatrices", boneMatrices.data(), boneMatrices.size());

    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].draw(viewProjMtx, shader);
		shader->set_vec3("DiffuseColor", &color[0]);

    }

    glUseProgram(0);
}

void Model::debug_draw(const glm::mat4& viewProjMtx, Shader* shader) {
	// Draw hitbox
    //std::cout << "debug draw\n";

    shader->activate();
	shader->set_mat4("model", (float*)&model);

    // Print model
	//std::cout << "Model matrix: " << glm::to_string(model) << std::endl;

	shader->set_mat4("viewProj", (float*)&viewProjMtx);

    // Draw the hitbox as a wireframe
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    hitbox->draw(viewProjMtx, shader);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Also draw hitboxes of meshes
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].hitbox->draw(viewProjMtx, shader);
	}

    glUseProgram(0);
}

void Model::load_model(const std::string &path)
{   

    Assimp::Importer importer;

    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    glm::mat4 identity = glm::mat4(1.0f);
    process_node(scene->mRootNode, scene, identity);

    // Create a hitbox for the model
    hitbox = new Cube(glm::vec3(min_x, min_y, min_z), glm::vec3(max_x, max_y, max_z));

    // Print hitbox
	std::cout << "Hitbox min: " << min_x << " " << min_y << " " << min_z << std::endl;
	std::cout << "Hitbox max: " << max_x << " " << max_y << " " << max_z << std::endl;

    // writeBoundingBoxToTextFile(glm::vec3(min_x, min_y, min_z), glm::vec3(max_x, max_y, max_z));
}

void Model::loadAnimations(const std::map<AnimationState, std::string> &animationPath)
{
    for (const auto &pair : animationPath)
    {
        AnimationState state = pair.first;
        const std::string path = pair.second;

        Assimp::Importer importer;

        const aiScene *scene = importer.ReadFile(path, aiProcess_LimitBoneWeights | aiProcess_Triangulate);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
            return;
        }

        globalInverse = glm::inverse(aiMatrixToGlm(scene->mRootNode->mTransformation));

        struct Node
        {
            aiNode *src = nullptr;
            int parentIndex = -1;

            Node(aiNode *src, int parentIndex) : src(src), parentIndex(parentIndex) {}
        };

        std::vector<AnimationNode> aniNodes;
        std::map<std::string, int> nodeNames;
        std::stack<Node> nodeStack;

        nodeStack.push(Node{scene->mRootNode, -1});

        while (!nodeStack.empty())
        {
            Node currentNode = std::move(nodeStack.top());
            nodeStack.pop();

            AnimationNode aniNode;
            aniNode.parentIndex = currentNode.parentIndex;

            aniNode.transformation = aiMatrixToGlm(currentNode.src->mTransformation);
            aniNode.hasBone = false;
            aniNode.name = std::string(currentNode.src->mName.data);

            assert(currentNode.parentIndex < static_cast<int>(aniNodes.size()));

            aniNodes.push_back(aniNode);

            nodeNames[aniNode.name] = static_cast<int>(aniNodes.size() - 1);

            int parentIndex = static_cast<int>(aniNodes.size() - 1);

            for (unsigned int i = 0; i < currentNode.src->mNumChildren; i++)
            {
                nodeStack.push(Node{currentNode.src->mChildren[i], parentIndex});
            }
        }

        if (scene->mNumAnimations > 0)
        {

            aiAnimation *ai_anim = scene->mAnimations[0];

            duration = ai_anim->mDuration;
            ticks = ai_anim->mTicksPerSecond != 0 ? ai_anim->mTicksPerSecond : 25.0f;

            std::vector<AnimationNode> nodes = aniNodes;
            animations[state] = nodes;

            for (unsigned int j = 0; j < ai_anim->mNumChannels; j++)
            {
                aiNodeAnim *ai_channel = ai_anim->mChannels[j];
                std::string boneName = ai_channel->mNodeName.C_Str();

                int boneIndex = skeleton.getBoneIndex(boneName);

                if (nodeNames.find(boneName) != nodeNames.end())
                {

                    AnimationNode node = aniNodes[nodeNames[boneName]];
                    node.hasBone = true;

                    for (unsigned int k = 0; k < ai_channel->mNumPositionKeys; k++)
                    {
                        aiVector3D pos = ai_channel->mPositionKeys[k].mValue;
                        float timeStamp = ai_channel->mPositionKeys[k].mTime;
                        node.positions.push_back(std::make_pair(timeStamp, aiVectorToGlm(pos)));
                    }

                    for (unsigned int k = 0; k < ai_channel->mNumRotationKeys; k++)
                    {
                        aiQuaternion rot = ai_channel->mRotationKeys[k].mValue;
                        float timeStamp = ai_channel->mRotationKeys[k].mTime;
                        node.rotations.push_back(std::make_pair(timeStamp, aiQuaternionToGlm(rot)));
                    }

                    for (unsigned int k = 0; k < ai_channel->mNumScalingKeys; k++)
                    {
                        aiVector3D scale = ai_channel->mScalingKeys[k].mValue;
                        float timeStamp = ai_channel->mScalingKeys[k].mTime;
                        node.scales.push_back(std::make_pair(timeStamp, aiVectorToGlm(scale)));
                    }

                    animations[state][nodeNames[boneName]] = node;
                }
            }
        }
    }
}

void Model::process_node(aiNode *node, const aiScene *scene, const glm::mat4 &parentTransform)
{
    glm::mat4 nodeTransform = aiMatrixToGlm(node->mTransformation);
    glm::mat4 globalTransform = parentTransform * nodeTransform;

    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(process_mesh(mesh, scene, globalTransform));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        process_node(node->mChildren[i], scene, globalTransform);
    }
    
}

Mesh Model::process_mesh(aiMesh *mesh, const aiScene *scene, const glm::mat4 &transform)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

	float mmin_x = INT_MAX, mmin_y = INT_MAX, mmin_z = INT_MAX, mmax_x = INT_MIN, mmax_y = INT_MIN, mmax_z = INT_MIN;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        glm::vec4 pos = transform * glm::vec4(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1.0);
        vertex.position = glm::vec3(pos);
        glm::vec3 norm = glm::mat3(transform) * glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        vertex.normal = norm;
        vertex.texCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);

        min_x = std::min(min_x, vertex.position.x);
        min_y = std::min(min_y, vertex.position.y);
        min_z = std::min(min_z, vertex.position.z);
        max_x = std::max(max_x, vertex.position.x);
        max_y = std::max(max_y, vertex.position.y);
        max_z = std::max(max_z, vertex.position.z);
        
        // Local minimum (per mesh)
		mmin_x = std::min(mmin_x, vertex.position.x);
		mmin_y = std::min(mmin_y, vertex.position.y);
		mmin_z = std::min(mmin_z, vertex.position.z);
		mmax_x = std::max(mmax_x, vertex.position.x);
		mmax_y = std::max(mmax_y, vertex.position.y);
		mmax_z = std::max(mmax_z, vertex.position.z);
    }

    // Process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    if (mesh->HasBones())
    {
        for (unsigned int i = 0; i < mesh->mNumBones; i++)
        {
            aiBone *bone = mesh->mBones[i];
            std::string boneName = bone->mName.C_Str();
            int boneIndex = 0;

            if (skeleton.boneMapping.find(boneName) == skeleton.boneMapping.end())
            {
                glm::mat4 boneOffset = aiMatrixToGlm(bone->mOffsetMatrix);
                boneIndex = skeleton.addBone(boneName, boneOffset);
            }
            else
            {
                boneIndex = skeleton.getBoneIndex(boneName);
            }

            for (unsigned int j = 0; j < bone->mNumWeights; j++)
            {
                aiVertexWeight weight = bone->mWeights[j];
                unsigned int vertexID = weight.mVertexId;
                float w = weight.mWeight;

                Vertex &vertex = vertices[vertexID];

                vertex.addBoneData(boneIndex, w);
            }
        }
    }
	Mesh m = Mesh(vertices, indices, textures);
	m.hitbox = new Cube(glm::vec3(mmin_x, mmin_y, mmin_z), glm::vec3(mmax_x, mmax_y, mmax_z));
	m.hitbox->set_color(glm::vec3(1.0f, 0.0f, 0.0f));
    // floorModel = true;
    if (floorModel) {
        // writeBoundingBoxToTextFile(glm::vec3(mesh->mAABB.mMin.x, mesh->mAABB.mMin.y, mesh->mAABB.mMin.z), glm::vec3(mesh->mAABB.mMax.x, mesh->mAABB.mMax.y, mesh->mAABB.mMax.z));
        // writeBoundingBoxToTextFile(glm::vec3(min_x, min_y, min_z), glm::vec3(max_x, max_y, max_z));
        writeBoundingBoxToTextFile(glm::vec3(mmin_x, mmin_y, mmin_z), glm::vec3(mmax_x, mmax_y, mmax_z));
    }
    return m;
}

void Model::updateAnimations(float deltaTime, AnimationState currentState)
{

    if (animations.find(currentState) == animations.end())
    {
        return;
    }

    static float timeSinceStart = 0.0f;
    timeSinceStart += deltaTime;
    float ticksPerSecond = ticks != 0 ? ticks : 25.0f;
    float timeInTicks = timeSinceStart * ticksPerSecond;
    float animationTime = fmod(timeInTicks, duration);

    for (int i = 0; i < animations[currentState].size(); i++)
    {
        AnimationNode &node = animations[currentState][i];

        glm::mat4 localTransform = node.transformation;

        if (node.hasBone)
        {
            glm::mat4 posMatrix = interpolatePosition(animationTime, node);
            glm::mat4 rotMatrix = interpolateRotation(animationTime, node);
            glm::mat4 scaleMatrix = interpolateScale(animationTime, node);

            localTransform = posMatrix * rotMatrix * scaleMatrix;
        }

        glm::mat4 globalTransform = glm::mat4(1.0f);
        if (node.parentIndex != -1)
        {
            AnimationNode &parent = animations[currentState][node.parentIndex];
            globalTransform = parent.boneTrans;
        }

        globalTransform = globalTransform * localTransform;
        node.boneTrans = globalTransform;

        int boneIndex = skeleton.getBoneIndex(node.name);

        if (boneIndex != -1)
        {
            skeleton.bones[boneIndex].finalTransformation = globalInverse * globalTransform * skeleton.bones[boneIndex].boneOffset;
        }
    }
}

glm::mat4 Model::interpolatePosition(float time, const AnimationNode &node)
{
    if (node.positions.size() == 1)
    {
        return glm::translate(glm::mat4(1.0f), node.positions[0].second);
    }
    int p0Index = -1, p1Index = -1;
    for (size_t i = 0; i < node.positions.size() - 1; i++)
    {
        if (time < node.positions[i + 1].first)
        {
            p0Index = i;
            p1Index = i + 1;
            break;
        }
    }
    if (p0Index == -1)
    {
        return glm::translate(glm::mat4(1.0f), node.positions[0].second);
    }
    float startTime = node.positions[p0Index].first;
    float endTime = node.positions[p1Index].first;
    float factor = (time - startTime) / (endTime - startTime);
    glm::vec3 interpolatedPosition = glm::mix(node.positions[p0Index].second, node.positions[p1Index].second, factor);
    return glm::translate(glm::mat4(1.0f), interpolatedPosition);
}

glm::mat4 Model::interpolateRotation(float time, const AnimationNode &node)
{
    if (node.rotations.size() == 1)
    {
        return glm::mat4_cast(node.rotations[0].second);
    }
    int r0Index = -1, r1Index = -1;
    for (size_t i = 0; i < node.rotations.size() - 1; i++)
    {
        if (time < node.rotations[i + 1].first)
        {
            r0Index = i;
            r1Index = i + 1;
            break;
        }
    }
    if (r0Index == -1)
    {
        return glm::mat4_cast(node.rotations[0].second);
    }
    float startTime = node.rotations[r0Index].first;
    float endTime = node.rotations[r1Index].first;
    float factor = (time - startTime) / (endTime - startTime);
    glm::quat interpolatedRotation = glm::slerp(node.rotations[r0Index].second, node.rotations[r1Index].second, factor);
    return glm::mat4_cast(interpolatedRotation);
}

glm::mat4 Model::interpolateScale(float time, const AnimationNode &node)
{
    if (node.scales.size() == 1)
    {
        return glm::scale(glm::mat4(1.0f), node.scales[0].second);
    }
    int s0Index = -1, s1Index = -1;
    for (size_t i = 0; i < node.scales.size() - 1; i++)
    {
        if (time < node.scales[i + 1].first)
        {
            s0Index = i;
            s1Index = i + 1;
            break;
        }
    }
    if (s0Index == -1)
    {
        return glm::scale(glm::mat4(1.0f), node.scales[0].second);
    }
    float startTime = node.scales[s0Index].first;
    float endTime = node.scales[s1Index].first;
    float factor = (time - startTime) / (endTime - startTime);
    glm::vec3 interpolatedScale = glm::mix(node.scales[s0Index].second, node.scales[s1Index].second, factor);
    return glm::scale(glm::mat4(1.0f), interpolatedScale);
}

glm::mat4 Model::aiMatrixToGlm(const aiMatrix4x4 &from)
{
    glm::mat4 to;
    to[0][0] = from.a1;
    to[1][0] = from.a2;
    to[2][0] = from.a3;
    to[3][0] = from.a4;
    to[0][1] = from.b1;
    to[1][1] = from.b2;
    to[2][1] = from.b3;
    to[3][1] = from.b4;
    to[0][2] = from.c1;
    to[1][2] = from.c2;
    to[2][2] = from.c3;
    to[3][2] = from.c4;
    to[0][3] = from.d1;
    to[1][3] = from.d2;
    to[2][3] = from.d3;
    to[3][3] = from.d4;
    return to;
}

glm::quat Model::aiQuaternionToGlm(const aiQuaternion &aiQuat)
{
    return glm::quat(aiQuat.w, aiQuat.x, aiQuat.y, aiQuat.z);
}

glm::vec3 Model::aiVectorToGlm(const aiVector3D &aiVec)
{
    return glm::vec3(aiVec.x, aiVec.y, aiVec.z);
}

Model::~Model()
{
    delete hitbox;

	for (Mesh m : meshes) {
		//delete m;
		delete m.hitbox;
	}
}