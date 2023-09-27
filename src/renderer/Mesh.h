#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "Texture.h"


const int MAX_BONE = 4;


struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
    glm::vec3 tangent;
    glm::vec3 bitangent;
    int m_boneId[MAX_BONE];
    int m_weights[MAX_BONE];
};


class DMesh {
public:
    DMesh();
    ~DMesh();

    void Draw();

private:
    void SetupMesh();

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<DTexture*> textures;

    unsigned int VBO, EBO;
};