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
    ~DMaterial();

    void SetTexture(const std::string& t_type, DTexture *texture);
    void SetTextureDiffuse(DTexture *texture);
    void SetTextureNormal(DTexture *texture);
    void SetBaseColor(const glm::vec4& color);
    void SetShader(DShader *shader);
    void SetName(const std::string& new_name);

    void SetMatrix(const std::string& m_type, glm::mat4 mx);

    void ApplyMaterial(DLight *light);

    std::unordered_map<E_TEXTURE_TYPE, DTexture *> textures;
    std::string material_name;

    DTexture *diffuse;
    DTexture *normal;

    DShader *shader;
    glm::vec4 base_color;

    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;

    glm::vec3 camera_pos;

    bool  isSpecular;
    float specularity;
    float shininess;

};

#endif