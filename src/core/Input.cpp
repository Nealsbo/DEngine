#include "Input.h"

DInputHandler::DInputHandler() {}

DInputHandler::~DInputHandler() {}

void DInputHandler::Init(GLFWwindow * dwindow) {
    this->window = dwindow;
}

void DInputHandler::Update() {
    glfwPollEvents();
}

bool DInputHandler::IsKeyDown(int key) {
    if (glfwGetKey(window, key) == GLFW_PRESS) {
        return true;
    }
    
    return false;
}
