#pragma once

#include "../core/WindowManager.h"
#include "../core/Scene.h"

#include "Shader.h"

// counts average fps for given amount of frames
const int deltaBufferSize = 10;

class DRenderEngine {
public:
	DRenderEngine();
	~DRenderEngine();

	int  Init(DWindowManager * wm);
	void Shutdown();

	void DrawFrame(DScene *scene, float delta);
	void DrawAvgFps();

	void BeginFrame();
	void EndFrame();

	bool LoadFont();
	void RenderText(std::string text, float x, float y, float scale);
	void PrintDebugMsg(const std::string& message);

private:
	DWindowManager * win;

  	GLuint VAO, VBO;
	
	DShader *textShader;
	DCamera *camera;

	char buffer[99999];
	char text[16] = "helloworld";

	float avgFrameDelta[deltaBufferSize] = { 0 };
	int currentFrameDelta = 0;
};

struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};