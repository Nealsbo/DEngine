#ifndef __MODEL_H__
#define __MODEL_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"
#include "Light.h"
#include "Material.h"

#include "../../ext/tiny_gltf.h"

class DModel {
public:
    DModel();
    ~DModel();

    void Draw(const glm::mat4& camMat, const glm::vec3& camPos, DLight *light);
    void DrawMesh(const std::map<int, GLuint>& ebos, tinygltf::Mesh& mesh);
    void DrawModelNodes(tinygltf::Node& node);
    void SetShader(DShader * _shader);
    void CreateMaterial(DMaterial *mat);

    void AddPosition(glm::vec3 &pos);
    void AddRotation(glm::vec3 &rot);

    void SetPosition(glm::vec3 &pos);
    void SetRotation(glm::vec3 &rot);
    void SetScale(glm::vec3 &s);

    glm::vec3 GetPosition();
    glm::vec3 GetRotation();
    glm::vec3 GetScale();


    void LoadModel(const std::string &fileName);
    std::pair<GLuint, std::map<int, GLuint>> SetupModel();
    void SetupModelNodes(std::map<int, GLuint>& ebos, tinygltf::Model &model, tinygltf::Node &node);
    void SetupMesh(std::map<int, GLuint>& ebos, tinygltf::Model &model, tinygltf::Mesh &mesh);

    void PrintModel();

private:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    DMaterial *material;

    DShader   *shader;
    DMesh     *mesh;
    std::vector<DTexture *> textures;

    std::pair<unsigned int, std::map<int, unsigned int>> VAO_and_EBOs;
	unsigned int TextureID;

    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;
};

#endif