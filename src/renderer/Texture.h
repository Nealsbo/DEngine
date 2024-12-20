#pragma once

#include <glad/glad.h>

#include <string>

enum E_TEXTURE_TYPE{
    ETT_DIFFUSE = 1,
    ETT_NORMAL,
    ETT_SPECULAR,
    ETT_LIGHTMAP,
    ETT_NONE
};

class DTexture {
public:
    DTexture();
    ~DTexture();

    void Generate(unsigned int width, unsigned int height, unsigned char* data);
    void Bind() const;

    void PrintTexInfo();

    unsigned int ID;
    unsigned int Width, Height;
    unsigned int Internal_Format;
    unsigned int Image_Format;
    unsigned int Wrap_S;
    unsigned int Wrap_T;
    unsigned int Filter_Min;
    unsigned int Filter_Max;
    unsigned int type;

    std::string name;
};
