#version 450

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inTextureCoord;
layout (location = 3) in vec3 inNormal;

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
    gl_Position = uniformBufferObject.projection * uniformBufferObject.view * uniformBufferObject.model * vec4(inPosition, 1.0);
    fragColor = inColor;

    textureCoord = inTextureCoord;
    normal = uniformBufferObject.normalInverse * inNormal;
    fragPos = vec3(vec4(inPosition, 1.0) * uniformBufferObject.model);
}
