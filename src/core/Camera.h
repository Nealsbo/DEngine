#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float CAM_SPEED   =  4.0f;
const float CAM_SENS    =  0.1f;
const float DEF_FOV     =  60.0f;

class DCamera {
public:
    DCamera();
    ~DCamera();

    void SetPosition(glm::vec3 &new_position);
    void AddPosition(glm::vec3 &new_position);
    glm::vec3 GetPosition();

    void Move(int forward, int right, float delta);

    void SetSpeed(float cam_speed);
    float GetSpeed();

    void SetFov(float new_fov);
    float GetFov();

    glm::mat4 GetViewMatrix();
    void ProcessMouse(float x, float y, bool constrainPitch);

    void UpdateVectors();

private:
    glm::vec3 position;
    glm::vec3 world_up;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;

    float yaw;
    float pitch;
    float fov;
    float speed;
    float sens;
};