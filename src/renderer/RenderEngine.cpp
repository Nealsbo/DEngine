#include "RenderEngine.h"

const char* minVertexShaderSource = "#version 400 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* minFragmentShaderSource = "#version 400 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

DRenderEngine::DRenderEngine() {}

DRenderEngine::~DRenderEngine() {}

void DRenderEngine::Init(DWindowManager * wm) {
    win = wm;

    shader = new DShader("../assets/base.vs", "../assets/base.fs");

    float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void DRenderEngine::Shutdown() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    delete shader;
}

void DRenderEngine::Draw(DCamera &camera) {
    glClearColor(0.3f, 0.0f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->Use();

    glm::mat4 projection = glm::perspective(glm::radians(camera.GetFov()), 1280.0f / 720.0f, 0.1f, 100.0f);
    shader->SetMat4("projection", projection);

    glm::mat4 view = camera.GetViewMatrix();
    shader->SetMat4("view", view);

    glm::mat4 model = glm::mat4(1.0f);
    shader->SetMat4("model", model);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    win->SwapBuffers();
}

void DRenderEngine::SetModel() {

}

void DRenderEngine::SetMesh() {

}

void DRenderEngine::SetTexture() {

}

void DRenderEngine::SetShader() {

}
