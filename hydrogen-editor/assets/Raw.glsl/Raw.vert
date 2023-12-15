#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
} ubo;

layout(binding = 4) uniform ObjectInformation {
    mat4 model;
} objectInfo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoords;

layout(location = 0) out vec3 fragPosition;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec2 fragTexCoords;

void main() {
    fragPosition = vec3(objectInfo.model * vec4(inPosition, 1.0));
    fragNormal = mat3(transpose(inverse(objectInfo.model))) * inNormal;
    fragTexCoords = inTexCoords;

    gl_Position = ubo.proj * ubo.view * vec4(fragPosition, 1.0);
}
