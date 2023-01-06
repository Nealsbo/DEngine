#pragma once

#include <GLFW/glfw3.h>

class DInputHandler {
public:
    DInputHandler();
    ~DInputHandler();

    void Init(GLFWwindow* window);
    void Shutdown();
    void Update();

    bool IsKeyDown(int key);

private:
    GLFWwindow * window = nullptr;
};

