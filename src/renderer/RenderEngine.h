#pragma once

#include "../core/WindowManager.h"
#include "../core/Camera.h"

#include "Shader.h"

#include "../../ext/tiny_gltf.h"
class DRenderEngine {
public:
	DRenderEngine();
	~DRenderEngine();

	int Init(DWindowManager * wm);
	void Shutdown();

	void Draw(DCamera &camera);

	void SetModel();

	void SetMesh();
	void SetTexture();
	void SetShader();

	void LoadModel();

private:
	DWindowManager * win;

	unsigned int VBO, VAO;
	DShader *shader;

	char buffer[99999];
	char text[16] = "helloworld";

};

