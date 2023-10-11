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

void DWindowManager::Shutdown() {
	SDL_Quit();
}

void DWindowManager::SwapBuffers() {
	SDL_GL_SwapWindow(window);
}

SDL_Window* DWindowManager::GetWindow() {
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

	const char* description = NULL;

	result = SDL_Init(SDL_INIT_VIDEO);
    if(result < 0) {
        printf("Failed to init SDL: %i\n", result);
        description = SDL_GetError();
        if (description)
            printf("%i | %s", result, description);
		return -1;
    }

	window = SDL_CreateWindow("SDL App", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);
    if (window == NULL) {
        printf("ERROR: Failed to create SDL window\n");
        SDL_Quit();
        return -1;
    }

	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 0 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	SDL_GL_SetSwapInterval(1);

	glContext = SDL_GL_CreateContext( window );
    if( glContext == NULL ) {
        printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
    }

	// todo window resize sdl
	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		printf("ERROR: Failed to initialize GLAD\n");
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);

	width = screenWidth;
	height = screenHeight;
	glViewport(0, 0, width, height);
	
	return 0;
}

/*
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
*/
