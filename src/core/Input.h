#pragma once

#include <GLFW/glfw3.h>
#include <SDL2/SDL.h>

#include <string>


class DKey {
public:
    DKey();

    bool pressed = false;
    std::string bind = "";
};

class DInputHandler {
public:
    DInputHandler();
    ~DInputHandler();

    int Init(SDL_Window* window);
    void Shutdown();
    void Update();

    void SetKeyBinding(int key, std::string bind);
    std::string GetKeyBinding(int key);

    float GetMouseXOffset();
    float GetMouseYOffset();

    bool IsKeyDown(int key);

private:
    void UpdateMousePosition();

    SDL_Window * window = nullptr;

    double mousex;
    double mousey;
    double mousexOffset;
    double mouseyOffset;
};

