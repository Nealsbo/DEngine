#include "Material.h"

DMaterial::DMaterial() {
    material_name = "Blank";
}

DMaterial::~DMaterial() {
    if(shader)
        delete shader;
}

void DMaterial::SetTexture(const std::string& t_type, DTexture *texture) {
    textures[GetTypeByString(t_type)] = texture;
    //textures.push_back(texture);
}

void DMaterial::SetTextureDiffuse(DTexture *texture) {
    diffuse = texture;
}

void DMaterial::SetTextureNormal(DTexture *texture) {
    normal = texture;
}

void DMaterial::SetBaseColor(const glm::vec4& color) {
    base_color = color;
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
    glBindTexture(GL_TEXTURE_2D, textures[ETT_DIFFUSE]->ID);
    //glBindTexture(GL_TEXTURE_2D, diffuse->ID);
}

E_TEXTURE_TYPE GetTypeByString(const std::string& t_type) {
    if(t_type == "diffuse")
        return ETT_DIFFUSE;
    if(t_type == "normal")
        return ETT_NORMAL;
    if(t_type == "specular")
        return ETT_SPECULAR;
    if(t_type == "lightmap")
        return ETT_LIGHTMAP;
    
    return ETT_NONE;
}