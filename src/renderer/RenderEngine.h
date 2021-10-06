#pragma once

#include "../core/WindowManager.h"

class DRenderEngine {
public:
	DRenderEngine();
	~DRenderEngine();

	void Init();
	void Shutdown();

	void Render();

	int num_quads;
	unsigned int VBO, VAO;
	unsigned int shaderProgram;
	char buffer[99999];

	char text[16] = "helloworld";
};

