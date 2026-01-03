#include "Material.h"

DMaterial::DMaterial() {
    material_name = "Blank";
}

DMaterial::DMaterial(const std::string& mat_name) {
    material_name = mat_name;
    
    if(mat_name == "BaseMaterial") {
        DShader *_shader = new DShader("../assets/shaders/base.vs", "../assets/shaders/base.fs");
        SetShader(_shader);
    }
}

DMaterial::~DMaterial() {
    if(diffuse)
        delete diffuse;
    if(normal)
        delete normal;
    if(occlusion)
        delete occlusion;
    if(metalRoughness)
        delete metalRoughness;
    if(emissive)
        delete emissive;
    if(specular)
        delete specular;

    if(shader)
        delete shader;
}


void DMaterial::SetTexture(const std::string& t_type, DTexture *texture) {
    SetTexture(GetTypeByString(t_type), texture);
}

void DMaterial::SetTexture(E_TEXTURE_TYPE t_type, DTexture *texture) {
    //textures[GetTypeByString(t_type)] = texture;
    switch(t_type) {
        case ETT_DIFFUSE:
            diffuse = texture;
            break;
        case ETT_NORMAL:
            normal = texture;
            break;
        case ETT_EMISSIVE:
            emissive = texture;
            break;
        case ETT_OCCLUSION:
            occlusion = texture;
            break;
        case ETT_METALROUGH:
            metalRoughness = texture;
            break;
        case ETT_SPECULAR:
            specular = texture;
            break;
        case ETT_LIGHTMAP:
            //TODO
        case ETT_NONE:
        default:
            break;
    }
    //textures.push_back(texture);
}

void DMaterial::SetBaseColor(const glm::vec4& color) {
    baseColorValue = color;
}

void DMaterial::SetEmissiveColor(const glm::vec4& color) {
    emissiveValue = color;
}

void DMaterial::SetMetalness(const float& value) {
    metalness = value;
}

void DMaterial::SetRoughness(const float& value) {
    roughness = value;
}

void DMaterial::SetShader(DShader *new_shader) {
    shader = new_shader;
}

void DMaterial::SetName(const std::string& new_name) {
    material_name = new_name;
}

void DMaterial::SetMatrix(const std::string& m_type, glm::mat4 mx) {

}

void DMaterial::ApplyMaterial(DLight *light) {
    shader->Use();

    shader->SetMat4("Proj", projection);
    shader->SetMat4("View", view);
    shader->SetMat4("Model", model);
    shader->SetVec3("viewPos", camera_pos);

    // light properties
    shader->SetVec3("lightPos", light->GetPosition());
    shader->SetVec3("lightColor", light->GetColor());

    // material properties
    if(isSpecular) {
        shader->SetFloat("material.specular", shininess);
        shader->SetFloat("material.shininess", 64.0f);
    }

    glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, textures[ETT_DIFFUSE]->ID);
    glBindTexture(GL_TEXTURE_2D, diffuse->ID);
}

E_TEXTURE_TYPE GetTypeByString(const std::string& t_type) {
    if(t_type == "diffuse")
        return ETT_DIFFUSE;
    if(t_type == "normal")
        return ETT_NORMAL;
    if(t_type == "specular")
        return ETT_SPECULAR;
    if(t_type == "occlusion")
        return ETT_OCCLUSION;
    if(t_type == "metalrough")
        return ETT_METALROUGH;
    if(t_type == "emissive")
        return ETT_EMISSIVE;
    if(t_type == "lightmap")
        return ETT_LIGHTMAP;
    
    return ETT_NONE;
}