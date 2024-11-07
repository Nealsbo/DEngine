#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <string>
#include <map>

#include "Texture.h"
#include "Shader.h"
#include "Light.h"


class DMaterial {
public:
    DMaterial();
    ~DMaterial();

    void SetTexture(const std::string& t_type, DTexture *texture);
    void SetShader(DShader *shader);
    void SetName(const std::string& new_name);

    void SetMatrix(const std::string& m_type, glm::mat4 mx);

    void ApplyMaterial(DLight *light);

    //std::map<E_TEXTURE_TYPE, DTexture *> textures;
    std::vector<DTexture *> textures;
    DShader *shader;

    std::string material_name;

    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;

    glm::vec3 camera_pos;

    bool  isSpecular;
    float specularity;
    float shininess;
};

#endif