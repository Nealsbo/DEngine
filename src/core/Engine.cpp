#include "Engine.h"
#include "Logger.h"


extern bool isOnExitRequest;

int   frameCount     = 0;

Uint64 currFrameCounter = 0;
Uint64 lastFrameCounter = 0;
float deltaTime         = 0.0f;
float totalTime         = 0.0f;


void DEngine::PreInit(int argc, char** argv) {
    Log::Msg("Engine PreInit", LOG_LEVEL::INFO);
}

void DEngine::Init() {
    Log::Msg("Engine Init", LOG_LEVEL::INFO);

	int result;

    windowManager = new DWindowManager();
	input         = new DInputHandler();
	console       = new DConsole();
	renderer      = new DRenderEngine();
	session       = new DSession();

    result = windowManager->Init();
	if(result) {
		Log::Msg("WindowManager init failed", LOG_LEVEL::ERROR);
		Shutdown();
	}

	result = input->Init(windowManager->GetWindow());
	if(result) {
		Log::Msg("InputHandler init failed", LOG_LEVEL::ERROR);
		Shutdown();
	}

	result = console->Init();
	if(result) {
		Log::Msg("Console init failed", LOG_LEVEL::ERROR);
		Shutdown();
	}

	result = renderer->Init(windowManager);
	if(result) {
		Log::Msg("Renderer init failed", LOG_LEVEL::ERROR);
		Shutdown();
	}

	result = session->Init();
	if(result) {
		Log::Msg("GameSession init failed", LOG_LEVEL::ERROR);
		Shutdown();
	}

	lastFrameCounter = SDL_GetPerformanceCounter();
	currFrameCounter = lastFrameCounter;
}

void DEngine::Shutdown() {
    Log::Msg("Engine Shutdown", LOG_LEVEL::INFO);

	if(session) {
		session->Shutdown();
		delete session;
	}
	
	if(input) {
		input->Shutdown();
		delete input;
	}
	
	if(renderer) {
		renderer->Shutdown();
		delete renderer;
	}
	
	if(console) {
		console->Shutdown();
		delete console;
	}
	
	if(windowManager) {
		windowManager->Shutdown();
    	delete windowManager;
	}

	printf("Engine stats:\nFrame count: %i\nConsumed time: %f\nAverage FPS: %f\n", 
		frameCount, totalTime / 1000.0f, (float)frameCount / (totalTime / 1000.0f));
}

void DEngine::Frame() {
    frameCount++;

	lastFrameCounter = currFrameCounter;
	currFrameCounter = SDL_GetPerformanceCounter();
	deltaTime = (float)((currFrameCounter - lastFrameCounter) * 1000 / (float)SDL_GetPerformanceFrequency());
	totalTime += deltaTime;

	// Handle commands and events
	//eventProcess();

	// TODO: Implementation

	// Handle input
	input->Update();
	
	// Game process update
	// TODO Move update to dedicated instance
	//session->Frame();
	SceneUpdate(deltaTime);

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
