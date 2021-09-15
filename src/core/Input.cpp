#include "Input.h"

extern bool isOnExitRequest;

DInputHandler::DInputHandler() {}

DInputHandler::~DInputHandler() {}

void DInputHandler::Init(GLFWwindow * dwindow) {
    this->window = dwindow;
}

void DInputHandler::Update() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        //glfwSetWindowShouldClose(window, true);
        isOnExitRequest = true;
    }

    glfwPollEvents();
}
