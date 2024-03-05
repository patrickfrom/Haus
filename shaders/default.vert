#version 450

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inTextureCoord;
layout (location = 3) in vec3 inNormal;

layout (push_constant) uniform Constants {
    vec3 position;
} constants;

layout (location = 0) out vec3 fragColor;
layout (location = 1) out vec2 textureCoord;
layout (location = 2) out vec3 normal;
layout (location = 3) out vec3 fragPos;

layout (set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
    mat3 normalInverse;
} uniformBufferObject;

void main() {
    gl_Position = uniformBufferObject.projection * uniformBufferObject.view  * vec4(vec3(uniformBufferObject.model * vec4(inPosition, 1.0) + vec4(constants.position, 0.0f)), 1.0);
    fragColor = inColor;

    textureCoord = inTextureCoord;
    normal = uniformBufferObject.normalInverse * inNormal;
    fragPos = vec3(vec4(inPosition, 1.0) * uniformBufferObject.model);
}
