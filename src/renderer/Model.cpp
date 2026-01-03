#include "Model.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../ext/tiny_gltf.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#define TEXTURE_CHECK(i) (i != -1)

DModel::DModel() {
    position = glm::vec3(0.0f);
    rotation = glm::vec3(0.0f);
    scale = glm::vec3(1.0f);
    model_name = "Default Model";
    isGLTFModel = false;
}

DModel::~DModel() {
    //for (auto tex : textures);
        //delete tex;
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
        material = new DMaterial("BaseMaterial");
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

    
    for(size_t i = 0; i < meshes.size(); i++) {
        meshes[i]->material->projection = projectionm;
        meshes[i]->material->view = viewm;
        meshes[i]->material->model = modelm;
        meshes[i]->material->camera_pos = camPos;
    
        //meshes[i]->material->ApplyMaterial(light);

        meshes[i]->Draw(light);
    }

    glBindVertexArray(0);
}
/*
void DModel::DrawMesh(tinygltf::Mesh& mesh) {
    for (size_t i = 0; i < mesh.primitives.size(); ++i) {
        tinygltf::Primitive primitive = mesh.primitives[i];
        tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebos.at(indexAccessor.bufferView));
        glDrawElements(primitive.mode, indexAccessor.count, indexAccessor.componentType, BUFFER_OFFSET(indexAccessor.byteOffset));
    }
}
*/
/*
void DModel::DrawModelNodes(tinygltf::Node& node) {
    if ((node.mesh >= 0) && (node.mesh < (int)model.meshes.size())) {
        DrawMesh(VAO_and_EBOs.second, model.meshes[node.mesh]);
    }
    
    for (size_t i = 0; i < node.children.size(); i++) {
        DrawModelNodes(model.nodes[node.children[i]]);
    }
}
*/

void DModel::LoadModel(const std::string &fileName) {
    bool ret = loader.LoadASCIIFromFile(&m_model, &err, &warn, fileName.c_str());
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

    SetupModel();
    //PrintModel();
}

void DModel::SetupModel() {
    std::map<int, GLuint> ebos;
    GLuint vao;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //create an ebo for each node in the scene
    const tinygltf::Scene& scene = m_model.scenes[m_model.defaultScene];

    std::vector<double> &pos = m_model.nodes[scene.nodes[0]].translation;
    if(pos.size()) {
        position = glm::vec3(pos[0], pos[1], pos[2]);
    }

    std::vector<double> &rot = m_model.nodes[scene.nodes[0]].rotation;
    if(rot.size()) {
        glm::quat rotq = glm::quat(rot[3], rot[0], rot[1], rot[2]);
        rotation = glm::degrees(glm::eulerAngles(rotq));
    }

    std::vector<double> &sc = m_model.nodes[scene.nodes[0]].scale;
    if(sc.size()) {
        scale = glm::vec3(sc[0], sc[1], sc[2]);
    }
    
    SetupMaterials(m_model);

    for (size_t i = 0; i < scene.nodes.size(); ++i) {
        assert((scene.nodes[i] >= 0) && (scene.nodes[i] < (int)m_model.nodes.size()));
        SetupModelNodes(m_model, m_model.nodes[scene.nodes[i]]);
    }

    glBindVertexArray(0);
    //cleanup: make sure only ebos are stored
}

void DModel::SetupModelNodes(tinygltf::Model &model, tinygltf::Node &node) {
    if ((node.mesh >= 0) && (node.mesh < (int)model.meshes.size())) {
        SetupMesh(model, model.meshes[node.mesh]);
    }

    for (size_t i = 0; i < node.children.size(); i++) {
        assert((node.children[i] >= 0) && (node.children[i] < (int)model.nodes.size()));
        SetupModelNodes(model, model.nodes[node.children[i]]);
    }
}

void DModel::SetupMesh(tinygltf::Model &model, tinygltf::Mesh &mesh) {
    printf("Mesh count: %lu\n", model.meshes.size());
    printf("Mesh primitives count: %lu\n", mesh.primitives.size());
    for (size_t i = 0; i < mesh.primitives.size(); i++) {
        tinygltf::Primitive primitive = mesh.primitives[i];

        tinygltf::Accessor& indAcc = model.accessors[primitive.indices];
        tinygltf::BufferView& indBufView = model.bufferViews[indAcc.bufferView];
        tinygltf::Buffer& indBuf = model.buffers[indBufView.buffer];
        const void* indData = indBuf.data.data() + indBufView.byteOffset + indAcc.byteOffset;
        int compSize = tinygltf::GetComponentSizeInBytes(indAcc.componentType);
        //int indStride = indAcc.ByteStride();
        
        // TODO: checks on exist
        int posAttr = mesh.primitives[i].attributes.find("POSITION")->second;
        int norAttr = mesh.primitives[i].attributes.find("NORMAL")->second;
        int texAttr = mesh.primitives[i].attributes.find("TEXCOORD_0")->second;
        int tanAttr = mesh.primitives[i].attributes.find("TANGENT")->second;

        const tinygltf::Accessor& posAcc = model.accessors[posAttr];
        const tinygltf::Accessor& norAcc = model.accessors[norAttr];
        const tinygltf::Accessor& texAcc = model.accessors[texAttr];
        const tinygltf::Accessor& tanAcc = model.accessors[tanAttr];

        const tinygltf::BufferView& posBufView = model.bufferViews[posAcc.bufferView];
        const tinygltf::BufferView& norBufView = model.bufferViews[norAcc.bufferView];
        const tinygltf::BufferView& texBufView = model.bufferViews[texAcc.bufferView];
        const tinygltf::BufferView& tanBufView = model.bufferViews[tanAcc.bufferView];

        const tinygltf::Buffer& posBuf = model.buffers[posBufView.buffer];
        const tinygltf::Buffer& norBuf = model.buffers[norBufView.buffer];
        const tinygltf::Buffer& texBuf = model.buffers[texBufView.buffer];
        const tinygltf::Buffer& tanBuf = model.buffers[tanBufView.buffer];

        const void* posData = posBuf.data.data() + posBufView.byteOffset + posAcc.byteOffset;
        const void* norData = norBuf.data.data() + norBufView.byteOffset + norAcc.byteOffset;
        const void* texData = texBuf.data.data() + texBufView.byteOffset + texAcc.byteOffset;
        const void* tanData = tanBuf.data.data() + tanBufView.byteOffset + tanAcc.byteOffset;
        
        int posStride = posAcc.ByteStride(posBufView);
        int norStride = norAcc.ByteStride(norBufView);
        int texStride = texAcc.ByteStride(texBufView);
        int tanStride = tanAcc.ByteStride(tanBufView);
        
        if (posStride == 0) posStride = tinygltf::GetComponentSizeInBytes(posAcc.componentType) * tinygltf::GetNumComponentsInType(posAcc.type);
        if (norStride == 0) norStride = tinygltf::GetComponentSizeInBytes(norAcc.componentType) * tinygltf::GetNumComponentsInType(norAcc.type);
        if (texStride == 0) texStride = tinygltf::GetComponentSizeInBytes(texAcc.componentType) * tinygltf::GetNumComponentsInType(texAcc.type);
        if (tanStride == 0) tanStride = tinygltf::GetComponentSizeInBytes(tanAcc.componentType) * tinygltf::GetNumComponentsInType(tanAcc.type);

        std::vector<Vertex> verts;
        verts.resize(posAcc.count);
        for(size_t i = 0; i < posAcc.count; i++) {
            const float* vpos = reinterpret_cast<const float*>(posData + i * posStride); 
            const float* vnor = reinterpret_cast<const float*>(norData + i * norStride);
            const float* vtex = reinterpret_cast<const float*>(texData + i * texStride);
            const float* vtan = reinterpret_cast<const float*>(tanData + i * tanStride);
            
            verts[i].position = glm::vec3(vpos[0], vpos[1], vpos[2]);
            verts[i].normal   = glm::vec3(vnor[0], vnor[1], vnor[2]);
            verts[i].uv       = glm::vec2(vtex[0], vtex[1]);
            //verts[i].tangent  = glm::vec3(vtan[0], vtan[1], vtan[2]);
        }
        
        std::vector<uint32_t> inds;
        inds.resize(indAcc.count);
        if(indAcc.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
            const unsigned short* src = static_cast<const unsigned short*>(indData);
            for (size_t i = 0; i < indAcc.count; ++i) {
                inds[i] = static_cast<uint32_t>(src[i]);
            }
        } else if (indAcc.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
            const unsigned char* src = static_cast<const unsigned char*>(indData);
            for (size_t i = 0; i < indAcc.count; ++i) {
                inds[i] = static_cast<uint32_t>(src[i]);
            }
        } else {
            const unsigned int* src = static_cast<const unsigned int*>(indData);
            for (size_t i = 0; i < indAcc.count; ++i) {
                inds[i] = static_cast<uint32_t>(src[i]);
            }
        }
        
        DMesh *nMesh = new DMesh(verts, inds, materials[primitive.material]);
        meshes.push_back(nMesh);
    }
}

void DModel::SetupMaterials(tinygltf::Model &model) {
    int ti = 0;
    if (model.textures.size() > 0) {
        printf("Textures count: %lu\n", model.textures.size());
    // fixme: Use material's baseColor
        for(size_t i = 0; i < model.textures.size(); i++) {
    
            tinygltf::Texture &tex = model.textures[i];
            if (tex.source > -1) {
                DTexture *texture = new DTexture;
    
                tinygltf::Image &image = model.images[tex.source];
                texture->name = image.uri;
    
                GLenum format = GL_RGBA;
                if (image.component == 1) {
                  format = GL_RED;
                } else if (image.component == 2) {
                  format = GL_RG;
                } else if (image.component == 3) {
                  format = GL_RGB;
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
    
                textures.push_back(texture);
                ti++;
            }
        }
    }
    
    if(model.materials.size() > 0) {
        printf("Materials count: %lu\n", model.materials.size());
        for (auto& material: model.materials) {
            int def_index = material.pbrMetallicRoughness.baseColorTexture.index;
            int rgh_index = material.pbrMetallicRoughness.metallicRoughnessTexture.index;
            int nrm_index = material.normalTexture.index;
            int ems_index = material.emissiveTexture.index;
            int occ_index = material.occlusionTexture.index;
    
            //printf("%i %i %i %i %i\n", def_index, rgh_index, nrm_index, ems_index, occ_index);
            DMaterial *mat = new DMaterial("BaseMaterial");
    
            if(TEXTURE_CHECK(def_index)) {
                mat->SetTexture("diffuse", textures[def_index]);
            } else {
                auto &f = material.pbrMetallicRoughness.baseColorFactor;
                mat->SetBaseColor(glm::vec4(f[0], f[1], f[2], f[3]));
            }
    
            if(TEXTURE_CHECK(nrm_index)) {
                mat->SetTexture("normal", textures[nrm_index]);
            }
    
            if(TEXTURE_CHECK(ems_index)) {
                mat->SetTexture("emissive", textures[ems_index]);
            } else {
                auto &f = material.emissiveFactor;
                mat->SetEmissiveColor(glm::vec4(f[0], f[1], f[2], 1.0f));
            }
    
            if(TEXTURE_CHECK(occ_index)) {
                mat->SetTexture("occlusion", textures[occ_index]);
            }
    
            if(TEXTURE_CHECK(rgh_index)) {
                mat->SetTexture("metalRough", textures[rgh_index]);
            } else {
                double &fm = material.pbrMetallicRoughness.metallicFactor;
                double &fr = material.pbrMetallicRoughness.roughnessFactor;
                mat->SetMetalness(float(fm));
                mat->SetMetalness(float(fr));
            }
    
            materials.push_back(mat);
        }
    }

    material = materials[2];
}

void DModel::PrintModel() {
    printf("=======\n-------Model info:-------\n");
    printf("Model position: %0.3f, %0.3f, %0.3f\n", position.x, position.y, position.z);
    printf("Model rotation: %0.3f, %0.3f, %0.3f\n", rotation.x, rotation.y, rotation.z);
    printf("Model scale: %0.3f, %0.3f, %0.3f\n", scale.x, scale.y, scale.z);
    for (auto &mesh : m_model.meshes) {
        printf("mesh : %s\n", mesh.name.c_str());
        for (auto &primitive : mesh.primitives) {
            const tinygltf::Accessor &indexAccessor = m_model.accessors[primitive.indices];
            printf("indexaccessor: count %lu, type %i\n", indexAccessor.count, indexAccessor.componentType);
            
            const tinygltf::Material &mat = m_model.materials[primitive.material];
            for (auto &mats : mat.values) {
                printf("mat : %s\n", mats.first.c_str());
            }

            for (auto &image : m_model.images) {
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

    //for (auto tex : textures) {
    //    tex->PrintTexInfo();
    //}
    printf("Loaded primitives count: %lu\n", meshes.size());
    printf("=======\n");
}