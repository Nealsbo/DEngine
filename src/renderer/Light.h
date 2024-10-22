#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../ext/tiny_gltf.h"

class DLight {
public:
    DLight();
    ~DLight();

    void SetRadius(float rad);
    void SetColor(glm::vec3 &c);
    void AddPosition(glm::vec3 &pos);
    void SetPosition(glm::vec3 &pos);
    void SetName(const std::string &n);
    float GetRadius();
    glm::vec3 GetPosition();
    glm::vec3 GetColor();
    std::string GetName();

private:
    glm::vec3 position;
    glm::vec3 color;
    float radius;

    std::string name;
};

#endif