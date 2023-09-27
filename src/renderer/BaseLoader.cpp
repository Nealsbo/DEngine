#include <iostream>
#include <sstream>
#include <fstream>

#include "BaseLoader.h"
#include "stb_image.h"

DShader DLoader::LoadShader(const char *vShaderFile, const char *fShaderFile, std::string name) {
    shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile);
    return shaders[name];
}

DShader DLoader::GetShader(std::string name) {
    return shaders[name];
}

DTexture DLoader::LoadTexture(const char *file, bool alpha, std::string name) {
    textures[name] = loadTextureFromFile(file, alpha);
    return textures[name];
}

DTexture DLoader::GetTexture(std::string name) {
    return textures[name];
}

void DLoader::Clear() {
    for (auto iter : shaders)
        glDeleteProgram(iter.second.ID);
    for (auto iter : textures)
        glDeleteTextures(1, &iter.second.ID);
}

DShader DLoader::loadShaderFromFile(const char *vShaderFile, const char *fShaderFile) {
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vertexShaderFile(vShaderFile);
    std::ifstream fragmentShaderFile(fShaderFile);
    std::stringstream vShaderStream, fShaderStream;
        
    vShaderStream << vertexShaderFile.rdbuf();
    fShaderStream << fragmentShaderFile.rdbuf();
        
    vertexShaderFile.close();
    fragmentShaderFile.close();
        
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    return DShader(vShaderCode, fShaderCode);
}

DTexture DLoader::loadTextureFromFile(const char *file, bool alpha) {
    DTexture texture;
    
    if (alpha) {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }

    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    texture.Generate(width, height, data);
    stbi_image_free(data);
    
    return texture;
}
