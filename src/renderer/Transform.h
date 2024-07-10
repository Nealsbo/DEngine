#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform {
public:
    Transform(){};
    Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale);
    Transform(const Transform& tr);
    ~Transform(){};

    void GetMatrix();

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::mat4 matrix;
}