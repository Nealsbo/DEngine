#pragma once

#include <cstdint>
#include <iostream>

#include <glad/glad.h>

#include <SDL2/SDL.h>

#include <glm/glm.hpp>

class DWindowManager {
public:
	DWindowManager();
	~DWindowManager();

	int  Init();
	void Shutdown();

	void SwapBuffers();
	glm::vec2 GetMousePos();
	float GetMouseScroll();

	int GrabMouse(bool val);

	SDL_Window* GetWindow();
	SDL_GLContext GetGLContext();

private:
	int InitGL();

	uint32_t screenWidth = 1280;
	uint32_t screenHeight = 720;
	
	SDL_Window *window;
	SDL_GLContext glContext;
};

/*
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffsets, double yoffsets);
*/
