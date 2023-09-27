#include "WindowManager.h"

float lastX;
float lastY;
float xoffset;
float yoffset;
float scrollOffset;
bool firstMouse = true;

DWindowManager::DWindowManager() {}

DWindowManager::~DWindowManager() {}

int DWindowManager::Init() {
	lastX = screenWidth / 2.0f;
	lastY = screenHeight / 2.0f;
	InitGL();
	return 0;
}

void DWindowManager::ShutDown() {
	glfwTerminate();
}

void DWindowManager::SwapBuffers() {
	glfwSwapBuffers(window);
}

GLFWwindow* DWindowManager::GetWindow() {
	return window;
}

glm::vec2 DWindowManager::GetMousePos() {
	return glm::vec2(xoffset, yoffset);
}

float DWindowManager::GetMouseScroll() {
	return scrollOffset;
}

int DWindowManager::InitGL() {
	int width, height;
	int result; 

	const char* description;

	result = glfwInit();
    if(!result) {
        printf("Failed to init GLFW: %i\n", result);
        int code = glfwGetError(&description);
 
        if (description)
            printf("%i | %s", code, description);
        return -1;
    }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(screenWidth, screenHeight, "OpenGL Renderer", NULL, NULL);
	if (window == NULL) {
		printf("ERROR: Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("ERROR: Failed to initialize GLAD\n");
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);

	glfwGetWindowSize(window, &width, &height);
	glViewport(0, 0, width, height);
	
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    xoffset = xpos - lastX;
    yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;
}

void scroll_callback(GLFWwindow* window, double xoffsets, double yoffsets)
{
    scrollOffset = yoffsets;
}