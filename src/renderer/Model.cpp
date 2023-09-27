#include "Model.h"


DModel::DModel() {

}

DModel::~DModel() {

}

void DModel::SetShader() {

}

void DModel::SetTexture() {

}

void DModel::SetPosition(glm::vec3 &position) {
    this->position = position;
}

void DModel::SetRotation(glm::vec3 &rotation) {
    this->rotation = rotation;
}

void DModel::SetScale(glm::vec3 &scale) {
    this->scale = scale;
}
