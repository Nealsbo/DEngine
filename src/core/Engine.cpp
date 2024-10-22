#include "Engine.h"
#include "Logger.h"


extern bool isOnExitRequest;

int   frameCount     = 0;

Uint64 currFrameCounter = 0;
Uint64 currFrameCounter2= 0;
Uint64 lastFrameCounter = 0;
float deltaTime         = 0.001f;
float deltaTime2        = 0.001f;
float totalTime         = 0.001f;
float cappedDeltaTime   = 0.001f;


void DEngine::PreInit(int argc, char** argv) {
    Log::Msg("Engine PreInit", LOG_LEVEL::INFO);
}

void DEngine::Init() {
    Log::Msg("Engine Init", LOG_LEVEL::INFO);

	int result;
	std::string new_scene;

    windowManager = new DWindowManager();
	input         = new DInputHandler();
	console       = new DConsole();
	renderer      = new DRenderEngine();
	session       = new DSession();
	scene         = new DScene();

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

	result = scene->Init();
	if(result) {
		Log::Msg("Scene init failed", LOG_LEVEL::ERROR);
		Shutdown();
	}

	lastFrameCounter = SDL_GetPerformanceCounter();
	currFrameCounter = lastFrameCounter;

	if(new_scene.empty()) {
		scene->LoadDefaultScene();
	} else {
		scene->LoadScene(new_scene);
	}
}

void DEngine::Shutdown() {
    Log::Msg("Engine Shutdown", LOG_LEVEL::INFO);

	if(scene) {
		scene->Shutdown();
		delete scene;
	}

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

	// Move scope to session
	{
		uint64_t time1 = SDL_GetPerformanceCounter();
		// Handle commands and events
		//eventProcess();

		// Handle input
		input->Update();

		// Game process update
		// TODO Move update to dedicated instance/session
		//session->Frame();
		scene->Update(deltaTime);

    	// Render update
		renderer->DrawFrame(scene, deltaTime);

		uint64_t time2 = SDL_GetPerformanceCounter();
		deltaTime2 = (float)((time2 - time1) * 1000 / (float)SDL_GetPerformanceFrequency());

		// frame cap
		float cappedFrameTime = 1000.0f / (float)frameCap;
		if(deltaTime2 < cappedFrameTime) {
			SDL_Delay((uint32_t)(cappedFrameTime - deltaTime2));
		}
	}

	//printf("Frame time: %.3f | Logic time: %.3f\n", deltaTime, deltaTime2);
}
/*
void DEngine::SceneUpdate(float delta) {
	glm::vec2 mousexy = windowManager->GetMousePos();
	//glm::vec3 adds = glm::vec3(0.0, 0.0, 0.005);
	//glfwGetCursorPos(windowManager->GetWindow(), &xpos, &ypos);
	mainCamera.ProcessMouse(mousexy.x, mousexy.y, true);
	//mainCamera.AddPosition(adds);
}
*/

bool DEngine::IsRunning() {
	return !isOnExitRequest;
}