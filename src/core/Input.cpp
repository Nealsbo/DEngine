#include "Input.h"

#include <vector>


extern bool isOnExitRequest;

const int MAX_KEYS = 128;

std::vector<DKey> keys(MAX_KEYS); 


DKey::DKey() {
    
}

/*
=== Input Handler part ===
*/

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

void DInputHandler::SetKeyBinding(int key, std::string bind) {

}

std::string DInputHandler::GetKeyBinding(int key) {
    return "";
}

bool DInputHandler::IsKeyDown(int key) {
    if (glfwGetKey(window, key) == GLFW_PRESS) {
        return true;
    }
    
    return false;
}
