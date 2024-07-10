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

void Transform::GetMatrix() {
    matrix = glm::mat4(1.0f);
    matrix = glm::translate(matrix, position);
    matrix = glm::rotate(matrix, angle_in_degrees, rotation);
    matrix = glm::scale(matrix, scale);

    return matrix;
}
