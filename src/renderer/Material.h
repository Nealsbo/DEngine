#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <string>
#include <unordered_map>

#include "Texture.h"
#include "Shader.h"
#include "Light.h"

E_TEXTURE_TYPE GetTypeByString(const std::string& t_type);

class DMaterial {
public:
    DMaterial();
    DMaterial(const std::string& mat_name);
    ~DMaterial();

    void SetTexture(const std::string& t_type, DTexture *texture);
    void SetTexture(E_TEXTURE_TYPE t_type, DTexture *texture);
    
    void SetBaseColor(const glm::vec4& color);
    void SetEmissiveColor(const glm::vec4& color);
    void SetMetalness(const float& value);
    void SetRoughness(const float& value);
    
    void SetShader(DShader *shader);
    void SetName(const std::string& new_name);

    void SetMatrix(const std::string& m_type, glm::mat4 mx);

    void ApplyMaterial(DLight *light);

    std::unordered_map<E_TEXTURE_TYPE, DTexture *> textures;
    std::string material_name;

    DTexture *diffuse        = nullptr;
    DTexture *normal         = nullptr;
    DTexture *emissive       = nullptr;
    DTexture *metalRoughness = nullptr;
    DTexture *occlusion      = nullptr;
    DTexture *specular       = nullptr;

    DShader  *shader         = nullptr;

    glm::vec4 baseColorValue = glm::vec4(1.0f);
    glm::vec4 emissiveValue  = glm::vec4(0.0f);
    float metalness          = 0.0f;
    float roughness          = 0.0f;

    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;

    glm::vec3 camera_pos;

    bool  isSpecular;
    float specularity;
    float shininess;

};

#endif