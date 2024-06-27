#pragma once

#include <glm/glm.hpp>

#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"


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
};