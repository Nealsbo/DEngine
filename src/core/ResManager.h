#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "../renderer/Texture.h"
#include "../renderer/Material.h"
#include "../renderer/Mesh.h"
#include "../renderer/Model.h"

class ResourceManager {
public:
    ResourceManager();
    ~ResourceManager();

    void LoadTexture();
    void LoadMaterial();
    void LoadGLTF();

private:
    std::unordered_map<std::string, DTexture *> textures;
    std::unordered_map<std::string, DMaterial *> materials;
    std::unordered_map<std::string, DModel *> models;
};