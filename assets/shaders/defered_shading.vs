#version 400 core
layout(location = 0) in vec3 in_vertex;
layout(location = 1) in vec2 in_texcoord;

out vec2 TexCoords;

void main() {
    TexCoords = in_texcoord;
    gl_Position = vec4(in_vertex, 1.0);
}