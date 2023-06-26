#version 330 core

layout(location = 0) in vec3 v_in_Pos;
layout(location = 1) in vec3 v_in_Normal;
layout(location = 2) in vec2 v_in_TexCoords;

// uniform mat4 u_ViewProj;
// uniform mat4 u_Model;

void main() {
    gl_Position = /*u_ViewProj * u_Model * */ vec4(v_in_Pos, 1.0);
}
