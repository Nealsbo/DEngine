#include "WindowManager.h"



DWindowManager::DWindowManager() {}

DWindowManager::~DWindowManager() {}

int DWindowManager::Init() {
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