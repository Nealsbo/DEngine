#include "Transform.h"

Transform::Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& s) {
    position = pos;
    rotation = rot;
    scale = s;
    matrix = GetMatrix();
}

Transform::Transform(const Transform& tr) {
    position = tr.position;
    rotation = tr.rotation;
    scale = tr.scale;
    matrix = tr.matrix;
}

glm::mat4 Transform::GetMatrix() {
    matrix = glm::mat4(1.0f);
    matrix = glm::translate(matrix, position);
    matrix = glm::rotate(matrix, 1.0f, rotation);
    matrix = glm::scale(matrix, scale);

    return matrix;
}
