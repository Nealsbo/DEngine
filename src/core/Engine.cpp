#include "Engine.h"
#include "Logger.h"

extern bool isOnExitRequest;

int   frameCount     = 0;

float currFrameTime  = 0.0f;
float lastFrameTime  = 0.0f;
float deltaFrameTime = 0.0f;
float totalTime      = 0.0f;

void DEngine::PreInit(int argc, char** argv) {
    Log::Msg("Engine PreInit", LOG_LEVEL::INFO);
}

void DEngine::Init() {
    Log::Msg("Engine Init", LOG_LEVEL::INFO);

    windowManager = new DWindowManager();
	input         = new DInputHandler();
	renderer      = new DRenderEngine();

    windowManager->Init();
	input->Init(windowManager->GetWindow());
	renderer->Init();
}

void DEngine::Shutdown() {
    Log::Msg("Engine Shutdown", LOG_LEVEL::INFO);

	renderer->Shutdown();
    windowManager->ShutDown();

	delete renderer;
	delete input;
    delete windowManager;
}

void DEngine::Frame() {
    frameCount++;

	// Calc timestamp and delta
	currFrameTime  = (float)glfwGetTime();
	deltaFrameTime = currFrameTime - lastFrameTime;
	lastFrameTime  = currFrameTime;
	totalTime     += deltaFrameTime;

	// TODO: Implementation
	// Handle input
	input->Update();

	// Handle commands and events
	// Game process update
	// scene->Update(deltaFrameTime);

    // Render update
	renderer->Render();
	
	windowManager->SwapBuffers();

	if (isOnExitRequest) {
		printf("Engine frame count: %i\nConsumed time: %f\nAverage FPS: %f\n", 
			frameCount, totalTime, (float)frameCount / totalTime);
	}
}
