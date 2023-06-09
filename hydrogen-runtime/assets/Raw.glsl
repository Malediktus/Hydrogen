#type vertex

#version 330 core

layout(location = 0) in vec3 v_in_Pos;
layout(location = 1) in vec3 v_in_Normal;
layout(location = 2) in vec2 v_in_TexCoords;

uniform mat4 u_ViewProj;
uniform mat4 u_Model;

void main() {
    gl_Position = u_ViewProj * u_Model * vec4(v_in_Pos, 1.0);
}

#type geometry

#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

void main() {
    for (int i = 0; i < 3; i++) {
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}

#type pixel

#version 330 core

out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
