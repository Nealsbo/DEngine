#include "Light.h"

DLight::DLight() {
    static uint32_t light_id = 0;
    char buff[32];
    sprintf(buff, "%s_%i", "PointLight", ++light_id);
    name = std::string(buff);
}

DLight::~DLight() {

}

void DLight::SetRadius(float rad) {
    radius = rad;
}

void DLight::SetColor(glm::vec3 &c) {
    color = c;
}

void DLight::AddPosition(glm::vec3 &pos) {
    position += pos;
}

void DLight::SetPosition(glm::vec3 &pos) {
    position = pos;
}

void DLight::SetName(const std::string &n) {
    name = n;
}

float DLight::GetRadius() {
    return radius;
}

glm::vec3 DLight::GetPosition() {
    return position;
}

glm::vec3 DLight::GetColor() {
    return color;
}

std::string DLight::GetName() {
    return name;
}
