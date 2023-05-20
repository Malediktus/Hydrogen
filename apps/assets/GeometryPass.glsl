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

in vec2 f_out_TexCoords;
in vec3 f_out_Normal;
in vec3 f_out_FragPos;

layout(location = 0) out vec3 WorldPosOut;
layout(location = 1) out vec3 DiffuseOut;
layout(location = 2) out vec3 NormalOut;
layout(location = 3) out vec3 TexCoordOut;

void main() {
    WorldPosOut = f_out_FragPos;
    DiffuseOut = vec3(1.0, 1.0, 1.0);
    NormalOut = normalize(f_out_Normal);
    TexCoordOut = vec3(f_out_TexCoords, 0.0);
}
