#ifndef SKELETON_H
#define SKELETON_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include <string>
#include <vector>

struct BoneInfo {
    glm::mat4 boneOffset;           // Offset matrix that transforms from mesh space to bone space
    glm::mat4 finalTransformation;  // Final transformation matrix applied to the vertex shader

    BoneInfo();
};

class Skeleton {
public:
    glm::mat4 globalInverseTransform;   // Transforms from model space to world space
    std::map<std::string, int> boneMapping; // Maps bone names to their index
    std::vector<BoneInfo> bones;        // Vector containing information for each bone

    int getBoneIndex(const std::string& name) const;
    int addBone(const std::string& name, const glm::mat4& offsetMatrix);
    void setGlobalInverseTransform(const glm::mat4& globalInv);
};

#endif // SKELETON_H
