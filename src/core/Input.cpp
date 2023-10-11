#include "Input.h"

#include <vector>


extern bool isOnExitRequest;

const int MAX_KEYS = 128;
float _lastX;
float _lastY;

//keymap
std::vector<DKey> keys(MAX_KEYS); 


DKey::DKey() {
    
}

/*
=== Input Handler part ===
*/

DInputHandler::DInputHandler() {}

DInputHandler::~DInputHandler() {}

int DInputHandler::Init(SDL_Window * dwindow) {
    this->window = dwindow;
    return 0;
}

void DInputHandler::Shutdown() {

}

void DInputHandler::Update() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        if(event.type == SDL_QUIT)
            isOnExitRequest = true;
        if(event.key.keysym.sym == SDLK_ESCAPE)
            isOnExitRequest = true;
    }
}

void DInputHandler::SetKeyBinding(int key, std::string bind) {

}

std::string DInputHandler::GetKeyBinding(int key) {
    return "";
}

float DInputHandler::GetMouseXOffset() {
    return (float)mousexOffset;
}
float DInputHandler::GetMouseYOffset() {
    return (float)mouseyOffset;
}

bool DInputHandler::IsKeyDown(int key) {
    /*
    if (glfwGetKey(window, key) == GLFW_PRESS) {
        return true;
    }
    
    return false;
    */
}

void DInputHandler::UpdateMousePosition() {
    //glfwGetCursorPos(window, &mousex, &mousey);

    mousexOffset = mousex - _lastX;
    mouseyOffset = _lastY - mousex;

    _lastX = mousex;
    _lastY = mousex;
}
