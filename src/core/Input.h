#pragma once

#include <GLFW/glfw3.h>

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

    void Init(GLFWwindow* window);
    void Shutdown();
    void Update();

    void SetKeyBinding(int key, std::string bind);
    std::string GetKeyBinding(int key);

    bool IsKeyDown(int key);

private:
    GLFWwindow * window = nullptr;
};

