#include <iostream>

#include "Texture.h"

DTexture::DTexture() {
    Width           = 0;
    Height          = 0;
    Internal_Format = GL_RGB;
    Image_Format    = GL_RGB;
    Wrap_S          = GL_REPEAT;
    Wrap_T          = GL_REPEAT;
    Filter_Min      = GL_LINEAR;
    Filter_Max      = GL_LINEAR;

    name = "Blank";

    glGenTextures(1, &this->ID);
}

DTexture::~DTexture() {}

void DTexture::Generate(unsigned int width, unsigned int height, unsigned char* data) {
    this->Width  = width;
    this->Height = height;

    glBindTexture(GL_TEXTURE_2D, this->ID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void DTexture::Bind() const {
    glBindTexture(GL_TEXTURE_2D, this->ID);
}

void DTexture::PrintTexInfo() {
    printf("=====\n---Texutre info---\n");
    printf("Tex name: %s\n", name.c_str());
    printf("Tex size: Width x Height: %i x %i\n", Width, Height);
    printf("=====\n");
}