#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

class GBuffer {
public:
    GBuffer();
    ~GBuffer();

    void Init(int w, int h);
    void Shutdown();

    void Bind(bool read);
    void GetTexture(int t);

    unsigned int framebuf;
    GLuint positionTex;
    GLuint normalTex;
    GLuint diffuseTex;
    GLuint depthTex;
    private:
};