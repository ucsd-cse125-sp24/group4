#include "../include/model.h"

#include <iostream>  // For std::cout
#include <string>    // For std::string
#include <Windows.h> // For GetCurrentDirectory and MAX_PATH
#include "model.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "../include/glm/gtx/string_cast.hpp"
#include <algorithm>  

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

void Model::draw(const glm::mat4 &viewProjMtx, Shader *shader)
{

    shader->activate();
    shader->set_mat4("model", (float *)&model);
    shader->set_mat4("viewProj", (float *)&viewProjMtx);
    shader->set_vec3("DiffuseColor", &color[0]);

    // std::cout << "Global Inverse Transform: " << glm::to_string(skeleton.globalInverseTransform) << std::endl;
    // for (const auto& bone : skeleton.bones) {
    //     std::cout << "Bone Offset: " << glm::to_string(bone.boneOffset) << std::endl;
    // }

    std::vector<glm::mat4> boneMatrices(skeleton.bones.size());
    std::cout << "size: " << skeleton.bones.size() << std::endl;
    for (size_t i = 0; i < skeleton.bones.size(); ++i)
    {
        boneMatrices[i] = skeleton.bones[i].finalTransformation;
        // std::cout<<glm::to_string(boneMatrices[i])<<std::endl;
    }
    shader->set_mat4_array("boneMatrices", boneMatrices.data(), boneMatrices.size());

    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].draw(viewProjMtx, shader);
    }

    // Draw the hitbox as a wireframe
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glm::mat4 t = glm::mat4(1.0f);
    // t[3] = model[3];
    // hitbox->set_world(t);
    // float scale = 1 / PLAYER_MODEL_SCALE;
    // glm::mat4 t = glm::scale(model, glm::vec3(scale, scale, scale));
    // hitbox->set_world(t);
    hitbox->draw(viewProjMtx, shader);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glUseProgram(0);
}

void Model::load_model(const std::string &path)
{
    Assimp::Importer importer;

    // Get current directory in Windows
    // char buffer[MAX_PATH];
    // GetCurrentDirectory(MAX_PATH, buffer);
    // std::string current_dir(buffer);
    // std::cout << "Current directory: " << current_dir << std::endl;

    unsigned int processFlags =
        aiProcess_CalcTangentSpace | // calculate tangents and bitangents if possible
        aiProcess_JoinIdenticalVertices |    // join identical vertices/ optimize indexing
        // aiProcess_ValidateDataStructure |    // perform a full validation of the loader's output
        aiProcess_Triangulate | // Ensure all verticies are triangulated (each 3 vertices are triangle)
        aiProcess_ConvertToLeftHanded |      // convert everything to D3D left handed space (by default right-handed, for OpenGL)
        aiProcess_SortByPType |              // ?
        aiProcess_ImproveCacheLocality |     // improve the cache locality of the output vertices
        aiProcess_RemoveRedundantMaterials | // remove redundant materials
        aiProcess_FindDegenerates |          // remove degenerated polygons from the import
        aiProcess_FindInvalidData |          // detect invalid model data, such as invalid normal vectors
        aiProcess_GenUVCoords |              // convert spherical, cylindrical, box and planar mapping to proper UVs
        aiProcess_TransformUVCoords |        // preprocess UV transformations (scaling, translation ...)
        aiProcess_FindInstances |            // search for instanced meshes and remove them by references to one master
        aiProcess_LimitBoneWeights |         // limit bone weights to 4 per vertex
        aiProcess_OptimizeMeshes |           // join small meshes, if possible;
        // aiProcess_PreTransformVertices |     //-- fixes the transformation issue.
        aiProcess_SplitByBoneCount | // split meshes with too many bones. Necessary for our (limited) hardware skinning shader
        aiProcess_GenBoundingBoxes | // generate bounding boxes from meshes
        0;

    // const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    const aiScene *scene = importer.ReadFile(path, processFlags);

    /*
     * Useful options: aiProcess_GenNormals, aiProcess_SplitLargeMeshes, aiProcess_OptimizeMeshes
     */

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
    std::cout << "Expected meshes: " << scene->mNumMeshes << ", Loaded meshes: " << meshes.size() << std::endl;
}

void Model::loadAnimations(const std::map<AnimationState, std::string> &animationPath)
{
    for (const auto &pair : animationPath)
    {
        loadAnimationFromPath(pair.first, pair.second);
    }
}

void Model::loadAnimationFromPath(AnimationState state, const std::string &path)
{

    Assimp::Importer importer;

    unsigned int processFlags =
        aiProcess_Triangulate |
        aiProcess_LimitBoneWeights |
        aiProcess_GenSmoothNormals |
        aiProcess_JoinIdenticalVertices |
        aiProcess_ImproveCacheLocality |
        aiProcess_RemoveRedundantMaterials |
        aiProcess_SplitLargeMeshes |
        aiProcess_FindInvalidData |
        aiProcess_ValidateDataStructure | 0;

    const aiScene *scene = importer.ReadFile(path, processFlags);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    glm::mat4 globalInverse = glm::inverse(aiMatrixToGlm(scene->mRootNode->mTransformation));
    if (glm::determinant(globalInverse) == 0)
    {
        std::cerr << "Invalid global inverse transform matrix." << std::endl;
    }
    else
    {
        skeleton.setGlobalInverseTransform(globalInverse);
    }

    std::cout << "Global Inverse Transform: " << glm::to_string(globalInverse) << std::endl;

    std::cout << "Number of animations found: " << scene->mNumAnimations << std::endl;
    if (scene->mNumAnimations > 0)
    {

        aiAnimation *ai_anim = scene->mAnimations[0];
        Animation anim;
        anim.name = ai_anim->mName.C_Str();
        anim.duration = ai_anim->mDuration;
        anim.ticksPerSecond = ai_anim->mTicksPerSecond != 0 ? ai_anim->mTicksPerSecond : 25.0f;

        for (unsigned int j = 0; j < ai_anim->mNumChannels; j++)
        {
            aiNodeAnim *ai_channel = ai_anim->mChannels[j];
            std::string boneName = ai_channel->mNodeName.C_Str();

            AnimationNode node;
            node.name = boneName;

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

            anim.channels[node.name] = node;
        }

        animations[state] = anim;
    }
    else
    {
        std::cerr << "No animations found in the file." << std::endl;
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
    std::vector<BoneData> bones(mesh->mNumVertices);

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec4 pos = transform * glm::vec4(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1.0);
        vertex.position = glm::vec3(pos);
        glm::vec3 norm = glm::mat3(transform) * glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        vertex.normal = norm;

        // Skip the textures for now
        // if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        //{
        //	glm::vec2 vec;
        //	vec.x = mesh->mTextureCoords[0][i].x;
        //	vec.y = mesh->mTextureCoords[0][i].y;
        //	vertex.texCoords = vec;
        //}
        // else
        vertex.texCoords = glm::vec2(0.0f, 0.0f);

        // vertex.boneIndices = glm::ivec4(0); // Initialize with default bone index
        // vertex.boneWeights = glm::vec4(0.0f);

        vertices.push_back(vertex);

        min_x = std::min(min_x, vertex.position.x);
        min_y = std::min(min_y, vertex.position.y);
        min_z = std::min(min_z, vertex.position.z);
        max_x = std::max(max_x, vertex.position.x);
        max_y = std::max(max_y, vertex.position.y);
        max_z = std::max(max_z, vertex.position.z);
    }

    // Process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    std::cout << "Processing mesh with " << mesh->mNumBones << " bones." << std::endl;

    for (unsigned int j = 0; j < mesh->mNumBones; j++)
    {
        aiBone *bone = mesh->mBones[j];
        std::string boneName(bone->mName.data);
        glm::mat4 boneOffset = aiMatrixToGlm(bone->mOffsetMatrix);
        int boneIndex;
        if (skeleton.boneMapping.find(boneName) == skeleton.boneMapping.end())
        {
            boneIndex = skeleton.addBone(boneName, boneOffset);
        }
        else
        {
            boneIndex = skeleton.getBoneIndex(boneName);
        }

        for (unsigned int k = 0; k < bone->mNumWeights; k++)
        {
            unsigned int vertexID = bone->mWeights[k].mVertexId;
            float weight = bone->mWeights[k].mWeight;

            // Add the bone data to the vertex
            bones[vertexID].addBoneData(boneIndex, weight);
        }
    }
    return Mesh(vertices, indices, textures, bones);
}

glm::vec3 Model::interpolatePosition(float time, const AnimationNode &node)
{
    if (node.positions.size() == 1)
    {
        return node.positions[0].second;
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
        return node.positions[0].second;
    }
    float startTime = node.positions[p0Index].first;
    float endTime = node.positions[p1Index].first;
    float factor = (time - startTime) / (endTime - startTime);
    return glm::mix(node.positions[p0Index].second, node.positions[p1Index].second, factor);
}

glm::quat Model::interpolateRotation(float time, const AnimationNode &node)
{
    if (node.rotations.size() == 1)
    {
        return node.rotations[0].second;
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
        return node.rotations[0].second;
    }
    float startTime = node.rotations[r0Index].first;
    float endTime = node.rotations[r1Index].first;
    float factor = (time - startTime) / (endTime - startTime);
    return glm::slerp(node.rotations[r0Index].second, node.rotations[r1Index].second, factor);
}

glm::vec3 Model::interpolateScale(float time, const AnimationNode &node)
{
    if (node.scales.size() == 1)
    {
        return node.scales[0].second;
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
        return node.scales[0].second;
    }
    float startTime = node.scales[s0Index].first;
    float endTime = node.scales[s1Index].first;
    float factor = (time - startTime) / (endTime - startTime);
    return glm::mix(node.scales[s0Index].second, node.scales[s1Index].second, factor);
}

void Model::updateAnimations(float deltaTime, AnimationState currentState)
{
    if (animations.find(currentState) == animations.end())
        return;

    Animation &anim = animations[currentState];
    static float timeSinceStart = 0.0f;
    timeSinceStart += deltaTime;
    float ticksPerSecond = anim.ticksPerSecond != 0 ? anim.ticksPerSecond : 25.0f;
    float timeInTicks = timeSinceStart * ticksPerSecond;
    float animationTime = fmod(timeInTicks, anim.duration);
    // std::cout << "Updating animation for state: " << static_cast<int>(currentState) << std::endl;
    // std::cout << "Animation Time: " << animationTime << " / Duration: " << anim.duration << std::endl;

    for (auto &channel : anim.channels)
    {
        const AnimationNode &node = channel.second;
        glm::vec3 interpolatedPosition = interpolatePosition(animationTime, node);
        glm::quat interpolatedRotation = interpolateRotation(animationTime, node);
        glm::vec3 interpolatedScale = interpolateScale(animationTime, node);

        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), interpolatedPosition);
        glm::mat4 rotationMatrix = glm::mat4_cast(interpolatedRotation);
        glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), interpolatedScale);
        glm::mat4 boneTransform = translationMatrix * rotationMatrix * scalingMatrix;

        int boneIndex = skeleton.getBoneIndex(node.name);
        if (boneIndex != -1)
        {
            skeleton.bones[boneIndex].finalTransformation = skeleton.globalInverseTransform * boneTransform * skeleton.bones[boneIndex].boneOffset;
            std::cout << "Bone: " << node.name << " Index: " << boneIndex << std::endl;
            std::cout << "Interpolated Position: " << glm::to_string(interpolatedPosition) << std::endl;
            std::cout << "Interpolated Rotation: " << glm::to_string(interpolatedRotation) << std::endl;
            std::cout << "Interpolated Scale: " << glm::to_string(interpolatedScale) << std::endl;
            std::cout << "Bone Offset Matrix: " << glm::to_string(skeleton.bones[boneIndex].boneOffset) << std::endl;

            std::cout << "Final Transformation Matrix: " << glm::to_string(skeleton.bones[boneIndex].finalTransformation) << std::endl;
        }
        else
        {
            // std::cerr << "Bone not found: " << node.name << std::endl;
        }
    }
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
}