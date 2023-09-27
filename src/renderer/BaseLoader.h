#pragma once


#include <string>
#include <map>

#include <glad/glad.h>
#include "Texture.h"
#include "Shader.h"
#include "Model.h"


struct DFileMeta {
    std::string fileName;
    uint32_t    fileSize;
    int         lastFileUpdate;
};

class DLoader {
public:
    DLoader() { }

    static DShader  LoadShader(const char *vShaderFile, const char *fShaderFile, std::string name);
    static DTexture LoadTexture(const char *file, bool alpha, std::string name);
    static DModel   LoadMesh(const char *file, std::string name);

    static DShader  GetShader(std::string name);
    static DTexture GetTexture(std::string name);
    static DModel   GetMesh(std::string name);

    static void     ReloadFile(std::string name);
    static void     CheckForUpdatedFiles();

    static void     Clear();

private:
    static DShader  loadShaderFromFile(const char *vShaderFile, const char *fShaderFile);
    static DTexture loadTextureFromFile(const char *file, bool alpha);
    
    static std::map<std::string, DShader>  shaders;
    static std::map<std::string, DTexture> textures;
    static std::map<std::string, DModel>   meshes;
};
