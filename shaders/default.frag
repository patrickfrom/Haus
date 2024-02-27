#version 450

layout (location = 0) in vec3 fragColor;
layout (location = 1) in vec2 textureCoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 fragPos;

layout (location = 0) out vec4 FragColor;

layout (binding = 1) uniform sampler2D textureSampler;

void main() {
    vec3 norm = normalize(normal);
    vec3 viewDirection = normalize(vec3(0.0f, -2.5f, 2.0f) - fragPos);

    vec3 lightDirection = normalize(-vec3(-0.2f, -1.0f, -0.3f));

    // Diffuse Shading
    float diff = max(dot(norm, lightDirection), 0.0);

    // Specular Shading
    vec3 halfwayDirection = normalize(lightDirection + viewDirection);
    float spec = pow(max(dot(normal, halfwayDirection), 0.0), 8.0);

    vec4 ambient = vec4(0.5, 0.5, 0.5, 1.0) * texture(textureSampler, textureCoord);
    vec4 diffuse = vec4(1.0, 1.0, 1.0, 1.0) * diff * texture(textureSampler, textureCoord) * 4.0;
    vec4 specular = vec4(1.0, 1.0, 1.0, 1.0) * spec * texture(textureSampler, textureCoord);

    FragColor = ambient + diffuse;
}