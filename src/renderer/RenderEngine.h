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

	bool LoadFont();
	void RenderText(DShader &shader, std::string text, float x, float y, float scale);
	void PrintDebugMsg(const std::string& message);

private:
	DWindowManager * win;

	float scale;
	//std::map<int, GLuint> ebos;

  	GLuint VAO, VBO;
	std::pair<unsigned int, std::map<int, unsigned int>> VAO_and_EBOs;
	
	DShader *shader;
	DShader *textshader;

	char buffer[99999];
	char text[16] = "helloworld";
};

struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};