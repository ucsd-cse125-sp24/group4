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
    mat4 boneTransform = 
        boneMatrices[boneIndices[0]] * boneWeights[0] +
        boneMatrices[boneIndices[1]] * boneWeights[1] +
        boneMatrices[boneIndices[2]] * boneWeights[2] +
        boneMatrices[boneIndices[3]] * boneWeights[3];

    vec4 pos = boneTransform * vec4(position, 1.0f);
    vec4 norm = boneTransform * vec4(normal, 0.0f);

    fragNormal = mat3(transpose(inverse(model))) * vec3(norm); 
    gl_Position = viewProj * model * pos;

}
