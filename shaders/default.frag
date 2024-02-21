#version 450

layout (location = 0) in vec3 fragColor;
layout (location = 1) in vec2 textureCoord;

layout (location = 0) out vec4 FragColor;

layout (binding = 1) uniform sampler2D textureSampler;

void main() {
    FragColor = texture(textureSampler, textureCoord);
}