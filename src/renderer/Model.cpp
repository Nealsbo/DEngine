#include "Model.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../ext/tiny_gltf.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

DModel::DModel() {
    position = glm::vec3(0.0f);
    rotation = glm::vec3(0.0f);
    scale = glm::vec3(1.0f);
}

DModel::~DModel() {
    if(shader)
        delete shader;

    glDeleteVertexArrays(1, &VAO_and_EBOs.first);
    for (auto it = VAO_and_EBOs.second.cbegin(); it != VAO_and_EBOs.second.cend();) {
        glDeleteBuffers(1, &VAO_and_EBOs.second[it->first]);
        VAO_and_EBOs.second.erase(it++);
    }
}

void DModel::SetShader(DShader * _shader) {
    shader = _shader;
}

void DModel::SetTexture() {

}

void DModel::AddPosition(glm::vec3 &pos) {
    position += pos;
}

void DModel::AddRotation(glm::vec3 &rot) {
    rotation += rot;
}

void DModel::SetPosition(glm::vec3 &pos) {
    position = pos;
}

void DModel::SetRotation(glm::vec3 &rot) {
    rotation = rot;
}

void DModel::SetScale(glm::vec3 &s) {
    scale = s;
}

glm::vec3 DModel::GetPosition() {
    return position;
}

glm::vec3 DModel::GetRotation() {
    return rotation;
}

glm::vec3 DModel::GetScale() {
    return scale;
}

void DModel::SetMaterial(DMaterial *mat) {
    material = mat;
}

void DModel::ApplyMaterial() {
    if(material->isSpecular) {
        shader->SetFloat("material.specular", material->shininess);
        shader->SetFloat("material.shininess", 64.0f);
    }
}

void DModel::Draw(const glm::mat4& camMat, const glm::vec3& camPos, DLight *light) {
    glm::mat4 projectionm = glm::perspective(glm::radians(60.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
    glm::mat4 viewm = camMat;
    glm::mat4 modelm = glm::mat4(1.0f);
    modelm = glm::translate(modelm, position);
    modelm = glm::scale(modelm, scale);
    //modelm = glm::rotate(modelm, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    //glm::mat4 mvp = projectionm * viewm * modelm;

    shader->Use();
    //shader->SetMat4("MVP", mvp);
    shader->SetMat4("Proj", projectionm);
    shader->SetMat4("View", viewm);
    shader->SetMat4("Model", modelm);
    shader->SetVec3("viewPos", camPos);

    // light properties
    shader->SetVec3("lightPos", light->GetPosition());
    shader->SetVec3("lightColor", light->GetColor());

    // material properties
    shader->SetVec3("material.specular", 0.8f, 0.8f, 0.8f);
    shader->SetFloat("material.shininess", 64.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureID);

    glBindVertexArray(VAO_and_EBOs.first);

    const tinygltf::Scene& scene = model.scenes[model.defaultScene];
    for (size_t i = 0; i < scene.nodes.size(); ++i) {
        DrawModelNodes(model.nodes[scene.nodes[i]]);
    }

    glBindVertexArray(0);
}

void DModel::DrawMesh(const std::map<int, GLuint>& ebos, tinygltf::Mesh& mesh) {
    for (size_t i = 0; i < mesh.primitives.size(); ++i) {
        tinygltf::Primitive primitive = mesh.primitives[i];
        tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebos.at(indexAccessor.bufferView));
        glDrawElements(primitive.mode, indexAccessor.count, indexAccessor.componentType, BUFFER_OFFSET(indexAccessor.byteOffset));
    }
}

void DModel::DrawModelNodes(tinygltf::Node& node) {
    if ((node.mesh >= 0) && (node.mesh < (int)model.meshes.size())) {
        DrawMesh(VAO_and_EBOs.second, model.meshes[node.mesh]);
    }

    for (size_t i = 0; i < node.children.size(); i++) {
        DrawModelNodes(model.nodes[node.children[i]]);
    }
}

void DModel::LoadModel(const std::string &fileName) {
    bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, fileName.c_str());
    if (!warn.empty()) {
        printf("Warn: %s\n", warn.c_str());
    }

    if (!err.empty()) {
        printf("Err: %s\n", err.c_str());
    }

    if (!ret) {
        printf("Failed to parse glTF\n");
    }

    PrintModel();
    VAO_and_EBOs = SetupModel();
}

std::pair<GLuint, std::map<int, GLuint>> DModel::SetupModel() {
    std::map<int, GLuint> ebos;
    GLuint vao;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //create an ebo for each node in the scene
    const tinygltf::Scene& scene = model.scenes[model.defaultScene];
    for (size_t i = 0; i < scene.nodes.size(); ++i) {
        assert((scene.nodes[i] >= 0) && (scene.nodes[i] < (int)model.nodes.size()));
        SetupModelNodes(ebos, model, model.nodes[scene.nodes[i]]);
    }

    glBindVertexArray(0);
    //cleanup: make sure only ebos are stored
    for (auto it = ebos.cbegin(); it != ebos.cend();) {
        tinygltf::BufferView bufferView = model.bufferViews[it->first];
        if (bufferView.target != GL_ELEMENT_ARRAY_BUFFER) {
            glDeleteBuffers(1, &ebos[it->first]);
          ebos.erase(it++);
        }
        else {
            ++it;
        }
    }

    return { vao, ebos };
}

void DModel::SetupModelNodes(std::map<int, GLuint>& ebos, tinygltf::Model &model, tinygltf::Node &node) {
    if ((node.mesh >= 0) && (node.mesh < (int)model.meshes.size())) {
        SetupMesh(ebos, model, model.meshes[node.mesh]);
    }

    for (size_t i = 0; i < node.children.size(); i++) {
        assert((node.children[i] >= 0) && (node.children[i] < (int)model.nodes.size()));
        SetupModelNodes(ebos, model, model.nodes[node.children[i]]);
    }
}

void DModel::SetupMesh(std::map<int, GLuint>& ebos, tinygltf::Model &model, tinygltf::Mesh &mesh) {
    for (size_t i = 0; i < model.bufferViews.size(); ++i) {
        const tinygltf::BufferView& bufferView = model.bufferViews[i];
        if (bufferView.target == 0) {
            printf("WARN: bufferView.target is zero\n");
            continue;  // Unsupported bufferView.
                       /*
                         From spec2.0 readme:
                         https://github.com/KhronosGroup/glTF/tree/master/specification/2.0
                                  ... drawArrays function should be used with a count equal to
                         the count            property of any of the accessors referenced by the
                         attributes            property            (they are all equal for a given
                         primitive).
                       */
        }

        const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

        GLuint ebo;
        glGenBuffers(1, &ebo);
        ebos[i] = ebo;

        glBindBuffer(bufferView.target, ebo);
        glBufferData(bufferView.target, bufferView.byteLength, &buffer.data.at(0) + bufferView.byteOffset, GL_STATIC_DRAW);
    }

    for (size_t i = 0; i < mesh.primitives.size(); ++i) {
        tinygltf::Primitive primitive = mesh.primitives[i];
        tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];

        for (auto& attrib : primitive.attributes) {
            tinygltf::Accessor accessor = model.accessors[attrib.second];
            int byteStride = accessor.ByteStride(model.bufferViews[accessor.bufferView]);
            glBindBuffer(GL_ARRAY_BUFFER, ebos[accessor.bufferView]);

            int size = 1;
            if (accessor.type != TINYGLTF_TYPE_SCALAR) {
                size = accessor.type;
            }

            int vaa = -1;
            if (attrib.first.compare("POSITION") == 0) vaa = 0;
            if (attrib.first.compare("NORMAL") == 0) vaa = 1;
            if (attrib.first.compare("TEXCOORD_0") == 0) vaa = 2;
            if (vaa > -1) {
                glEnableVertexAttribArray(vaa);
                glVertexAttribPointer(vaa, size, accessor.componentType, accessor.normalized ? GL_TRUE : GL_FALSE, byteStride, ((char*)NULL + accessor.byteOffset));
            }
            else
                printf("vaa missing: %s\n", attrib.first.c_str());
        }

        if (model.textures.size() > 0) {
        // fixme: Use material's baseColor
            tinygltf::Texture &tex = model.textures[0];

            if (tex.source > -1) {

                GLuint texid;
                glGenTextures(1, &texid);

                tinygltf::Image &image = model.images[tex.source];

                glBindTexture(GL_TEXTURE_2D, texid);
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

                GLenum format = GL_RGBA;

                if (image.component == 1) {
                  format = GL_RED;
                } else if (image.component == 2) {
                  format = GL_RG;
                } else if (image.component == 3) {
                  format = GL_RGB;
                } else {
                  // ???
                }

                GLenum type = GL_UNSIGNED_BYTE;
                if (image.bits == 8) {
                  // ok
                } else if (image.bits == 16) {
                  type = GL_UNSIGNED_SHORT;
                } else {
                  // ???
                }

                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, format, type, &image.image.at(0));
                TextureID = texid;
            }
        }
    }
}

void DModel::PrintModel() {
    for (auto &mesh : model.meshes) {
        std::cout << "mesh : " << mesh.name << std::endl;
        for (auto &primitive : mesh.primitives) {
            const tinygltf::Accessor &indexAccessor = model.accessors[primitive.indices];
            printf("indexaccessor: count %lu, type %i\n", indexAccessor.count, indexAccessor.componentType);

            const tinygltf::Material &mat = model.materials[primitive.material];
            for (auto &mats : mat.values) {
                printf("mat : %s\n", mats.first.c_str());
            }

            for (auto &image : model.images) {
                printf("image name : %s\n", image.uri.c_str());
                printf("  size : %lu\n", image.image.size());
                printf("  w/h  : %i/%i\n", image.width, image.height);
            }

            printf("indices   : %i\n", primitive.indices);
            printf("mode      : (%i)\n", primitive.mode);
            for (auto &attrib : primitive.attributes) {
                printf("attribute : %s\n", attrib.first.c_str());
            }
        }
    }
}