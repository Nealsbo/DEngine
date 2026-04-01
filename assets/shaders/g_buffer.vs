#version 400 core
layout (location = 0) in vec3 in_vertex;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texcoord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;

void main() {
    vec4 worldPos = Model * vec4(in_vertex, 1.0);
    FragPos = worldPos.xyz; 
    TexCoords = in_texcoord;
    
    mat3 normalMatrix = transpose(inverse(mat3(Model)));
    Normal = normalMatrix * in_normal;

    gl_Position = Proj * View * worldPos;
}

