#pragma once

#include <GLFW/glfw3.h>

class DInputHandler {
public:
    DInputHandler();
    ~DInputHandler();

    void Init(GLFWwindow* window);
    void Update();

private:
    GLFWwindow * window = nullptr;
};

