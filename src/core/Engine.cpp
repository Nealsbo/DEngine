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
	console       = new DConsole();
	renderer      = new DRenderEngine();
	session       = new DSession();

    windowManager->Init();
	input->Init(windowManager->GetWindow());
	console->Init();
	renderer->Init(windowManager);
	session->Init();

	lastFrameTime = (float)glfwGetTime();
}

void DEngine::Shutdown() {
    Log::Msg("Engine Shutdown", LOG_LEVEL::INFO);

	session->Shutdown();
	input->Shutdown();
	renderer->Shutdown();
	console->Shutdown();
    windowManager->ShutDown();

	// TODO: Remove deletion/Rework init
	delete session;
	delete renderer;
	delete console;
	delete input;
    delete windowManager;

	printf("Engine stats:\nFrame count: %i\nConsumed time: %f\nAverage FPS: %f\n", 
		frameCount, totalTime, (float)frameCount / totalTime);
}

void DEngine::Frame() {
    frameCount++;

	// Calc timestamp and delta
	currFrameTime  = (float)glfwGetTime();
	deltaFrameTime = currFrameTime - lastFrameTime;
	lastFrameTime  = currFrameTime;
	totalTime     += deltaFrameTime;

	// Handle commands and events
	//eventProcess();

	// TODO: Implementation

	// Handle input
	input->Update();
	
	// Game process update
	// TODO Move update to dedicated instance
	//session->Frame();
	SceneUpdate(deltaFrameTime);

    // Render update
	renderer->Draw(mainCamera);
}

void DEngine::SceneUpdate(float delta) {
	glm::vec2 mousexy = windowManager->GetMousePos();
	//glfwGetCursorPos(windowManager->GetWindow(), &xpos, &ypos);
	mainCamera.ProcessMouse(mousexy.x, mousexy.y, true);
}

void DEngine::LoadModels() {

}