#type vertex

#version 330 core

layout(location = 0) in vec3 v_in_Pos;
layout(location = 1) in vec3 v_in_Normal;
layout(location = 2) in vec2 v_in_TexCoords;

out vec2 v_out_TexCoords;
out vec3 v_out_Normal;
out vec3 v_out_FragPos;

uniform mat4 u_ViewProj;
uniform mat4 u_Model;

void main() {
    v_out_TexCoords = v_in_TexCoords;
    v_out_Normal = mat3(transpose(inverse(u_Model))) * v_in_Normal;
    v_out_FragPos = vec3(u_Model * vec4(v_in_Pos, 1.0));
    gl_Position = u_ViewProj * vec4(v_out_FragPos, 1.0);
}

#type geometry

#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2 v_out_TexCoords[];
in vec3 v_out_Normal[];
in vec3 v_out_FragPos[];

out vec2 f_out_TexCoords;
out vec3 f_out_Normal;
out vec3 f_out_FragPos;

void main() {
    for (int i = 0; i < 3; i++) {
        f_out_TexCoords = v_out_TexCoords[i];
        f_out_Normal = v_out_Normal[i];
        f_out_FragPos = v_out_FragPos[i];
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}

#type pixel

#version 330 core

#define NR_POINT_LIGHTS 4
#define NR_DIRECTIONAL_LIGHTS 4
#define NR_SPOT_LIGHTS 4

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 u_ViewPos;
uniform int u_NumPointLights;
uniform int u_NumDirectionalLights;
uniform int u_NumSpotLights;
uniform PointLight u_PointLights[NR_POINT_LIGHTS];
uniform DirectionalLight u_DirectionalLights[NR_DIRECTIONAL_LIGHTS];
uniform SpotLight u_SpotLights[NR_SPOT_LIGHTS];

in vec2 f_out_TexCoords;
in vec3 f_out_Normal;
in vec3 f_out_FragPos;
out vec4 FragColor;

vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec4 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    vec3 norm = normalize(f_out_Normal);
    vec3 viewDir = normalize(u_ViewPos - f_out_FragPos);
    vec4 result = vec4(0.0, 0.0, 0.0, 0.0);
    for (int i = 0; i < u_NumPointLights; i++)
        result += CalcPointLight(u_PointLights[i], norm, f_out_FragPos, viewDir);
    for (int i = 0; i < u_NumDirectionalLights; i++)
        result += CalcDirectionalLight(u_DirectionalLights[i], norm, viewDir);
    for (int i = 0; i < u_NumSpotLights; i++)
        result += CalcSpotLight(u_SpotLights[i], norm, f_out_FragPos, viewDir);

    float gamma = 2.2;
    FragColor.rgb = pow(result.rgb, vec3(1.0 / gamma));
}

vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    vec4 ambient = vec4(light.ambient, 1.0);
    vec4 diffuse = vec4(light.diffuse * diff, 1.0);
    vec4 specular = vec4(light.specular * spec, 1.0);

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec4 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
    vec4 ambient = vec4(light.ambient, 1.0);
    vec4 diffuse = vec4(light.diffuse * diff, 1.0);
    vec4 specular = vec4(light.specular * spec, 1.0);

    return (ambient + diffuse + specular);
}

vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec4 ambient = vec4(light.ambient, 1.0);
    vec4 diffuse = vec4(light.diffuse * diff, 1.0);
    vec4 specular = vec4(light.specular * spec, 1.0);

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}
