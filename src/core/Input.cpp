#include "Input.h"

extern bool isOnExitRequest;

DInputHandler::DInputHandler() {}

DInputHandler::~DInputHandler() {}

void DInputHandler::Init(GLFWwindow * dwindow) {
    this->window = dwindow;
}

void DInputHandler::Shutdown() {

}

void DInputHandler::Update() {
    glfwPollEvents();

    if (IsKeyDown(GLFW_KEY_ESCAPE)) {
		isOnExitRequest = true;
	}
}

bool DInputHandler::IsKeyDown(int key) {
    if (glfwGetKey(window, key) == GLFW_PRESS) {
        return true;
    }
    
    return false;
}
