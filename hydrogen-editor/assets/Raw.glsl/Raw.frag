#version 450

layout(binding = 1) uniform sampler2D texSampler;
layout(binding = 2) uniform LightData {
  vec3 lightPos;
  vec3 viewPos;
  vec3 lightColor;
  vec3 objectColor;
} lightData;

layout(location = 0) in vec3 fragPosition;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragTexCoords;

layout(location = 0) out vec4 outColor;

void main() {
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightData.lightColor;
  	
    // diffuse 
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(lightData.lightPos - fragPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightData.lightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(lightData.viewPos - fragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightData.lightColor;  
        
    vec3 result = (ambient + diffuse + specular) * lightData.objectColor;
    outColor = vec4(result, 1.0);
}
