#include <iostream>

#include "core/Engine.h"

bool isOnExitRequest = false;

DEngine  gEngine;
DEngine* engine = &gEngine;

void EngineInit(int argc, char ** argv) {
	engine->PreInit(argc, argv);

	engine->Init();
}

void EngineMainLoop() {
	while (!isOnExitRequest) {
		engine->Frame();
	}

	engine->Shutdown();
}

int main(int argc, char ** argv) {
	EngineInit(argc, argv);

	EngineMainLoop();

	return 0;
}