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
    model_name = "Default Model";
    isGLTFModel = false;
}

DModel::~DModel() {
    for (auto tex : textures)
        delete tex;

    if(isGLTFModel) {
        glDeleteVertexArrays(1, &VAO_and_EBOs.first);
        for (auto it = VAO_and_EBOs.second.cbegin(); it != VAO_and_EBOs.second.cend();) {
            glDeleteBuffers(1, &VAO_and_EBOs.second[it->first]);
            VAO_and_EBOs.second.erase(it++);
        }
    } else {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
}

void DModel::SetShader(DShader * _shader) {
    material->SetShader(_shader);
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

std::string& DModel::GetModelName() {
    return model_name;
}

void DModel::CreateMaterial(DMaterial *mat) {
    if(mat != nullptr) {
        material = mat;
    }
    else {
        material = new DMaterial();
        material->SetName("Standart Material");
        DShader *_shader = new DShader("../assets/shaders/base.vs", "../assets/shaders/base.fs");
        material->SetShader(_shader);
    }
}

void DModel::Draw(const glm::mat4& camMat, const glm::vec3& camPos, DLight *light) {
    glm::mat4 projectionm = glm::perspective(glm::radians(60.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
    glm::mat4 viewm = camMat;
    glm::mat4 modelm = glm::mat4(1.0f);
    modelm = glm::translate(modelm, position);
    modelm = glm::scale(modelm, scale);
    modelm = glm::rotate(modelm, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    modelm = glm::rotate(modelm, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    modelm = glm::rotate(modelm, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    //glm::mat4 mvp = projectionm * viewm * modelm;

    material->projection = projectionm;
    material->view = viewm;
    material->model = modelm;
    material->camera_pos = camPos;

    material->ApplyMaterial(light);

    if(isGLTFModel) {
        glBindVertexArray(VAO_and_EBOs.first);

        const tinygltf::Scene& scene = model.scenes[model.defaultScene];
        for (size_t i = 0; i < scene.nodes.size(); ++i) {
            DrawModelNodes(model.nodes[scene.nodes[i]]);
        }
    } else {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, verticesCount / 8);
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

void DModel::CreateQuadMesh() {
    static uint32_t quad_id = 0;
    char buff[32];
    sprintf(buff, "%s_%i", "Quad", ++quad_id);
    model_name = std::string(buff);

    // positions, colors, texture coords
    float vertices[] = {
         0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
         0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
         0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f
    };

    verticesCount = sizeof(vertices);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    CreateMaterial(nullptr);
    uint8_t dotTex[4] = {255, 255, 255, 255};
    DTexture *tex = new DTexture();
    tex->Generate(1, 1, dotTex);
    material->SetTexture("Diffuse", tex);
}

void DModel::CreateCubeMesh() {
    static uint32_t cube_id = 0;
    char buff[32];
    sprintf(buff, "%s_%i", "Cube", ++cube_id);
    model_name = std::string(buff);

    // fix direction on some faces
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    verticesCount = sizeof(vertices);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);                       // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));     // normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));     // texture coord attribute
    glEnableVertexAttribArray(2);

    CreateMaterial(nullptr);
    uint8_t dotTex[4] = {255, 0, 0, 255};
    DTexture *tex = new DTexture();
    tex->Generate(1, 1, dotTex);
    material->SetTexture("Diffuse", tex);
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

    model_path = fileName;
    
    std::string split = model_path.substr(model_path.find_last_of('/') + 1);
    model_name = split.substr(0, split.find_last_of('.'));

    isGLTFModel = true;

    CreateMaterial(nullptr);
    VAO_and_EBOs = SetupModel();
    PrintModel();
}

std::pair<GLuint, std::map<int, GLuint>> DModel::SetupModel() {
    std::map<int, GLuint> ebos;
    GLuint vao;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //create an ebo for each node in the scene
    const tinygltf::Scene& scene = model.scenes[model.defaultScene];

    std::vector<double> &pos = model.nodes[scene.nodes[0]].translation;
    if(pos.size()) {
        position = glm::vec3(pos[0], pos[1], pos[2]);
    }

    std::vector<double> &rot = model.nodes[scene.nodes[0]].rotation;
    if(rot.size()) {
        glm::quat rotq = glm::quat(rot[3], rot[0], rot[1], rot[2]);
        rotation = glm::degrees(glm::eulerAngles(rotq));
    }

    std::vector<double> &sc = model.nodes[scene.nodes[0]].scale;
    if(sc.size()) {
        scale = glm::vec3(sc[0], sc[1], sc[2]);
    }

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
            for(size_t i = 0; i < model.textures.size(); i++) {

                tinygltf::Texture &tex = model.textures[i];
                if (tex.source > -1) {
                    DTexture *texture = new DTexture;

                    tinygltf::Image &image = model.images[tex.source];
                    texture->name = image.uri;
                    printf("\n%s\n", texture->name.c_str());
                    printf("%s\n\n", model.materials[0].name.c_str());

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

                    texture->Image_Format = format;
                    texture->type = type;

                    texture->Generate(image.width, image.height, image.image.data());
                    material->SetTexture(texture->name, texture);
                    //textures.push_back(texture);
                }
            }
        }
    }
}

void DModel::PrintModel() {
    printf("=======\n-------Model info:-------\n");
    printf("Model position: %0.3f, %0.3f, %0.3f\n", position.x, position.y, position.z);
    printf("Model rotation: %0.3f, %0.3f, %0.3f\n", rotation.x, rotation.y, rotation.z);
    printf("Model scale: %0.3f, %0.3f, %0.3f\n", scale.x, scale.y, scale.z);
    for (auto &mesh : model.meshes) {
        printf("mesh : %s\n", mesh.name.c_str());
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

    for (auto tex : textures) {
        tex->PrintTexInfo();
    }
    printf("=======\n");
}