

#include "skeleton.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "../include/glm/gtx/string_cast.hpp"
#include <iostream>

BoneInfo::BoneInfo() 
    : boneOffset(glm::mat4(1.0f)), finalTransformation(glm::mat4(1.0f)) {}

int Skeleton::getBoneIndex(const std::string& name) const {
    auto it = boneMapping.find(name);
    if (it != boneMapping.end()) {
        return it->second;
    }
    return -1;
}

int Skeleton::addBone(const std::string& name, const glm::mat4& offsetMatrix) {
    int index = getBoneIndex(name);

    if (index == -1) { // Bone not found, add new
        index = bones.size();
        bones.push_back(BoneInfo());
        bones[index].boneOffset = offsetMatrix;
        boneMapping[name] = index;
        return index;
    }
    return index;
}