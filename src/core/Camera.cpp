#include "Camera.h"
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

DCamera::DCamera() {
    position = glm::vec3(0.0f, 0.0f, 5.0f);
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    world_up = up;

    yaw = YAW;
    pitch = PITCH;
    fov = DEF_FOV;
    speed = CAM_SPEED;
    sens = CAM_SENS;

    UpdateVectors();
}

DCamera::~DCamera() {

}

void DCamera::SetPosition(glm::vec3 &new_position) {
    position = new_position;
}

void DCamera::AddPosition(glm::vec3 &new_position) {
    position += new_position;
}

glm::vec3 DCamera::GetPosition() {
    return position;
}

void DCamera::Move(int forward_move, int right_move, int up_move, float delta)
{
    if(forward_move > 0)
        position += front * delta * speed;
    else if(forward_move < 0)
        position -= front * delta * speed;

    if(right_move > 0)
        position += right * delta * speed;
    else if(right_move < 0)
        position -= right * delta * speed;
    
    if(up_move > 0)
        position += up * delta * speed;
    else if(up_move < 0)
        position -= up * delta * speed;
}

void DCamera::SetSpeed(float cam_speed) {
    speed = cam_speed;
}

float DCamera::GetSpeed() {
    return speed;
}

void DCamera::SetFov(float new_fov) {
    fov = new_fov;
}

float DCamera::GetFov() {
    return fov;
}

glm::mat4 DCamera::GetViewMatrix() {
    return glm::lookAt(position, position + front, up);
}

void DCamera::ProcessMouse(float x, float y, bool constrainPitch = true) {
    x *= sens;
    y *= sens;

    yaw   += x;
    pitch += y;

    if (constrainPitch) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    UpdateVectors();
    //std::cout << glm::to_string(front) << std::endl;
}


void DCamera::UpdateVectors() {
    glm::vec3 f;
    f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    f.y = sin(glm::radians(pitch));
    f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(f);
    // also re-calculate the Right and Up vector
    right = glm::normalize(glm::cross(front, world_up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    up    = glm::normalize(glm::cross(right, front));
}