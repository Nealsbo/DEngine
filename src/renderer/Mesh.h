#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "Texture.h"
#include "Material.h"
#include "GBuffer.h"



const int MAX_BONE = 4;


struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
    //glm::vec3 tangent;
    //glm::vec3 bitangent;
    //int m_boneId[MAX_BONE];
    //float m_weights[MAX_BONE];
};


class DMesh {
public:
    DMesh();
    DMesh(std::vector<Vertex> &verts, std::vector<uint32_t> &inds, DMaterial *mat);
    ~DMesh();

    void Draw(DLight* light);
    void Draw(GBuffer& gBuf, DLight* light);
    DMaterial *material;

private:
    void SetupMesh();

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;


    unsigned int VBO, EBO, VAO;
};