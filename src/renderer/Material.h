#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <string>
#include <map>

#include "Texture.h"
#include "Shader.h"



class DMaterial {
public:
    DMaterial();
    ~DMaterial();

    void SetTexture(const std::string& t_type);
    void SetShader(DShader *shader);

    std::map<E_TEXTURE_TYPE, DTexture> textures;
    DShader *shader;

    bool  isSpecular;
    float specularity;
    float shininess;
};

#endif