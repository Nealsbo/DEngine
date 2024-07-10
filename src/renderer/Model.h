#ifndef __MODEL_H__
#define __MODEL_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"

#include "../../ext/tiny_gltf.h"

class DModel {
public:
    DModel();
    ~DModel();

    void Draw(const glm::mat4& camMat);
    void DrawMesh(const std::map<int, GLuint>& ebos, tinygltf::Mesh& mesh);
    void DrawModelNodes(tinygltf::Node& node);
    void SetShader(DShader * _shader);
    void SetTexture();

    void AddPosition(glm::vec3 &pos);
    void AddRotation(glm::vec3 &rot);

    void SetPosition(glm::vec3 &pos);
    void SetRotation(glm::vec3 &rot);
    void SetScale(glm::vec3 &s);

    void LoadModel(const std::string &fileName);
    std::pair<GLuint, std::map<int, GLuint>> SetupModel();
    void SetupModelNodes(std::map<int, GLuint>& ebos, tinygltf::Model &model, tinygltf::Node &node);
    void SetupMesh(std::map<int, GLuint>& ebos, tinygltf::Model &model, tinygltf::Mesh &mesh);

    void PrintModel();

private:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    DShader * shader;
    DMesh * mesh;
    DTexture * texture;

    std::pair<unsigned int, std::map<int, unsigned int>> VAO_and_EBOs;
	unsigned int TextureID;

    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;
};

#endif