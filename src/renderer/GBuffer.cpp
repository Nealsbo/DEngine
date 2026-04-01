#include "GBuffer.h"

#include <stdio.h>

GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        char* error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            //case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            //case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        printf("%s | %s (%i)\n", error, file, line);
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)


GBuffer::GBuffer() {

}

GBuffer::~GBuffer() {

}

void GBuffer::Init(int w, int h) {
    glGenFramebuffers(1, &framebuf);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuf);

    glGenTextures(1, &positionTex);
    glBindTexture(GL_TEXTURE_2D, positionTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, positionTex, 0);
    
    glGenTextures(1, &normalTex);
    glBindTexture(GL_TEXTURE_2D, normalTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normalTex, 0);
    
    glGenTextures(1, &diffuseTex);
    glBindTexture(GL_TEXTURE_2D, diffuseTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, diffuseTex, 0);
    
    GLenum DrawBuf[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, DrawBuf);

    glGenRenderbuffers(1, &depthTex);
    glBindRenderbuffer(GL_RENDERBUFFER, depthTex);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, w, h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthTex);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("ERROR: FrameBuffer status error\n");
        glCheckError();
        return;
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    printf("GBuffer init complete\n");
}

void GBuffer::Shutdown() {
    if(framebuf)
        glDeleteFramebuffers(1, &framebuf);
    
    if(diffuseTex)
        glDeleteTextures(1, &diffuseTex);
    
    if(positionTex)
        glDeleteTextures(1, &positionTex);

    if(normalTex)
        glDeleteTextures(1, &normalTex);

    if(depthTex)
        glDeleteTextures(1, &depthTex);
}

void GBuffer::Bind(bool read = true) {
    if(read)
        glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuf);
    else
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuf);
    //glBindFramebuffer(GL_FRAMEBUFFER, framebuf);
}

void GBuffer::GetTexture(int t) {

}
