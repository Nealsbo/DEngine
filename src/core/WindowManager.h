#pragma once

#include <cstdint>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

class DWindowManager {
public:
	DWindowManager();
	~DWindowManager();

	int  Init();
	void ShutDown();

	void SwapBuffers();
	glm::vec2 GetMousePos();
	float GetMouseScroll();

	GLFWwindow* GetWindow();

private:
	int InitGL();

	uint32_t screenWidth = 1280;
	uint32_t screenHeight = 720;
	
	GLFWwindow *window = nullptr;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffsets, double yoffsets);