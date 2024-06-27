#pragma once

#include "../core/WindowManager.h"
#include "../core/Scene.h"

#include "Shader.h"

#include "../../ext/tiny_gltf.h"
class DRenderEngine {
public:
	DRenderEngine();
	~DRenderEngine();

	int Init(DWindowManager * wm);
	void Shutdown();

	void DrawFrame(DScene *scene);

	void Draw();
	void DrawModelNodes(tinygltf::Node& node);
	void DrawMesh(const std::map<int, GLuint>& ebos, tinygltf::Mesh& mesh);

	void SetModel();

	void SetMesh();
	void SetTexture();
	void SetShader();

	void LoadModel(std::string &fileName);
	std::pair<unsigned int, std::map<int, unsigned int>> SetupModel(tinygltf::Model &model);
	void SetupModelNodes(std::map<int, GLuint>& ebos, tinygltf::Model &model, tinygltf::Node &node);
	void SetupMesh(std::map<int, GLuint>& ebos, tinygltf::Model &model, tinygltf::Mesh &mesh);

	void PrintModel(const tinygltf::Model &model);

private:
	DWindowManager * win;

	float scale;
	//std::map<int, GLuint> ebos;

	std::pair<unsigned int, std::map<int, unsigned int>> VAO_and_EBOs;
  	GLuint vao;
	DShader *shader;

	char buffer[99999];
	char text[16] = "helloworld";
};