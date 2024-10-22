#version 400 core
layout(location = 0) in vec3 in_vertex;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texcoord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

void main(){
	FragPos = vec3(Model * vec4(in_vertex, 1.0));
	//Normal = normalize(mat3(MVP) * in_normal);
	Normal = mat3(transpose(inverse(Model))) * in_normal;  
	TexCoords = in_texcoord;
	gl_Position = Proj * View * vec4(FragPos, 1);
}