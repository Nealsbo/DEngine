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
    void DrawMesh(tinygltf::Mesh& mesh);
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

    std::string& GetModelName();

    void CreateQuadMesh();
    void CreateCubeMesh();

    void LoadModel(const std::string &fileName);
    void SetupModel();
    void SetupModelNodes(tinygltf::Model &model, tinygltf::Node &node);
    void SetupMesh(tinygltf::Model &model, tinygltf::Mesh &mesh);
    void SetupMaterials(tinygltf::Model &model);

    void PrintModel();

private:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    DMaterial *material;
    DShader   *shader;
    DMesh     *mesh;
    
    std::vector<DMesh *> meshes;
    std::vector<DTexture *> textures;
    std::vector<DMaterial *>materials;

    bool isGLTFModel;

    tinygltf::Model m_model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    std::string model_path;
    std::string model_name;
};

#endif