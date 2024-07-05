#pragma once

#include <glm/glm.hpp>

#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"
/*
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../ext/tiny_gltf.h"
*/
class DModel {
public:
    DModel();
    ~DModel();

    void Draw();

    void SetShader();
    void SetTexture();

    void SetPosition(glm::vec3 &position);
    void SetRotation(glm::vec3 &rotation);
    void SetScale(glm::vec3 &scale);

private:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    DShader * shader;
    DMesh * mesh;
    DTexture * texture;

    //Model model;
    //TinyGLTF loader;
    std::string err;
    std::string warn;
};