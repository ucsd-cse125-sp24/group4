#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;
layout(location = 3) in ivec4 boneIndices;
layout(location = 4) in vec4 boneWeights;

uniform mat4 viewProj;
uniform mat4 model;
uniform mat4 boneMatrices[100]; // An array to store bone matrices

out vec3 fragNormal;

void main() {
    float totalWeight = boneWeights[0] + boneWeights[1] + boneWeights[2] + boneWeights[3];

    vec4 pos;
    vec4 norm;

    if (totalWeight > 0) {
        // Apply bone transformations if weights are non-zero
        mat4 boneTransform = 
            boneMatrices[boneIndices[0]] * boneWeights[0] +
            boneMatrices[boneIndices[1]] * boneWeights[1] +
            boneMatrices[boneIndices[2]] * boneWeights[2] +
            boneMatrices[boneIndices[3]] * boneWeights[3];

        pos = boneTransform * vec4(position, 1);
        norm = boneTransform * vec4(normal, 0);
    } else {
        // Use the original position and normal if weights are zero
        pos = vec4(position, 1);
        norm = vec4(normal, 0);
    }

    gl_Position = viewProj * model * pos;
    fragNormal = mat3(transpose(inverse(model))) * vec3(norm);
}
