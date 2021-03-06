#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

#include "WindowManager.h"
#include "Console.h"
#include "Input.h"
#include "../renderer/RenderEngine.h"

const uint32_t default_width  = 960;
const uint32_t default_height = 540;

class DEngine {
public:
	DEngine() {};
	~DEngine() {};

	void PreInit(int argc, char** argv);

	void Init();
	void Shutdown();

	void Frame();

private:
	DWindowManager * windowManager = nullptr;
	DInputHandler *  input         = nullptr;
	DConsole *       console       = nullptr;
	DRenderEngine *  renderer      = nullptr;
};

