#include "RenderEngine.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/gtc/type_ptr.hpp>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include "../../ext/tiny_gltf.h"

//#include "../../ext/stb_image.h"
//#include "../../ext/stb_image_write.h"
//#include "../../ext/json.hpp"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using namespace tinygltf;

Model model;
TinyGLTF loader;
std::string err;
std::string warn;

glm::mat4 projectionm;
glm::mat4 viewm;
glm::mat4 modelm;

std::map<GLchar, Character> Characters;

DRenderEngine::DRenderEngine() {}

DRenderEngine::~DRenderEngine() {
    glDeleteVertexArrays(1, &VAO_and_EBOs.first);
    for (auto it = VAO_and_EBOs.second.cbegin(); it != VAO_and_EBOs.second.cend();) {
        glDeleteBuffers(1, &VAO_and_EBOs.second[it->first]);
        VAO_and_EBOs.second.erase(it++);
    }
}

int DRenderEngine::Init(DWindowManager * wm) {
    win = wm;

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader = new DShader("../assets/shaders/base.vs", "../assets/shaders/base.fs");
    
    if(!LoadFont()) printf("ERROR: Font load failed!\n");

    modelm = glm::mat4(1.0f);

    return 0;
}

void DRenderEngine::Shutdown() {
    delete shader;
}

void DRenderEngine::DrawFrame(DScene *scene) {
    glClearColor(0.3f, 0.0f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera = scene->GetCamera();

    Draw();

    RenderText(std::string("Test debug text"), 25.0f, 720.0f-57.0f, 1.0f);

    win->SwapBuffers();
}

void DRenderEngine::DrawMesh(const std::map<int, GLuint>& ebos, tinygltf::Mesh& mesh) {
    for (size_t i = 0; i < mesh.primitives.size(); ++i) {
        tinygltf::Primitive primitive = mesh.primitives[i];
        tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebos.at(indexAccessor.bufferView));
        glDrawElements(primitive.mode, indexAccessor.count, indexAccessor.componentType, BUFFER_OFFSET(indexAccessor.byteOffset));
    }
}

void DRenderEngine::DrawModelNodes(tinygltf::Node& node) {
    if ((node.mesh >= 0) && (node.mesh < (int)model.meshes.size())) {
        DrawMesh(VAO_and_EBOs.second, model.meshes[node.mesh]);
    }

    for (size_t i = 0; i < node.children.size(); i++) {
        DrawModelNodes(model.nodes[node.children[i]]);
    }
}

void DRenderEngine::Draw() {
    projectionm = glm::perspective(glm::radians(camera->GetFov()), 1280.0f / 720.0f, 0.1f, 100.0f);
    viewm = camera->GetViewMatrix();
    modelm = glm::rotate(modelm, glm::radians(0.2f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 mvp = projectionm * viewm * modelm;

    shader->Use();
    shader->SetMat4("MVP", mvp);
    shader->SetVec3("sun_position", glm::vec3(3.0, 10.0, -5.0));
    shader->SetVec3("sun_color", glm::vec3(1.0));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureID);

    glBindVertexArray(VAO_and_EBOs.first);

    const tinygltf::Scene& scene = model.scenes[model.defaultScene];
    for (size_t i = 0; i < scene.nodes.size(); ++i) {
        DrawModelNodes(model.nodes[scene.nodes[i]]);
    }

    glBindVertexArray(0);
}

void DRenderEngine::SetModel() {}
void DRenderEngine::SetMesh() {}
void DRenderEngine::SetTexture() {}
void DRenderEngine::SetShader() {}

void DRenderEngine::LoadModel(std::string &fileName) {
    //bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, "../assets/glTF/Suzanne.gltf");
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

    PrintModel(model);
    VAO_and_EBOs = SetupModel(model);
}

std::pair<GLuint, std::map<int, GLuint>> DRenderEngine::SetupModel(tinygltf::Model &model) {
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

void DRenderEngine::SetupModelNodes(std::map<int, GLuint>& ebos, tinygltf::Model &model, tinygltf::Node &node) {
    if ((node.mesh >= 0) && (node.mesh < (int)model.meshes.size())) {
        SetupMesh(ebos, model, model.meshes[node.mesh]);
    }

    for (size_t i = 0; i < node.children.size(); i++) {
        assert((node.children[i] >= 0) && (node.children[i] < (int)model.nodes.size()));
        SetupModelNodes(ebos, model, model.nodes[node.children[i]]);
    }
}

void DRenderEngine::SetupMesh(std::map<int, GLuint>& ebos, tinygltf::Model &model, tinygltf::Mesh &mesh) {
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

void DRenderEngine::PrintModel(const tinygltf::Model &model) {
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

bool DRenderEngine::LoadFont() {
    bool success = true;

    textshader = new DShader("../assets/shaders/text.vs", "../assets/shaders/text.fs");
    glm::mat4 projection = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f);
    textshader->Use();
    glUniformMatrix4fv(glGetUniformLocation(textshader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft)) {
        printf("ERROR::FREETYPE: Could not init FreeType Library\n");
        return -1;
    }

	// find path to font
    std::string font_name = "../assets/fonts/CourierPrime.ttf";
    if (font_name.empty()) {
        printf("ERROR::FREETYPE: Failed to load font_name\n");
        return -1;
    }
	
	// load font as face
    FT_Face face;
    if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
        printf("ERROR::FREETYPE: Failed to load font\n");
        return -1;
    }
    else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++) {
            // Load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                printf("ERROR::FREETYTPE: Failed to load Glyph\n");
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	return success;
}

void DRenderEngine::PrintDebugMsg(const std::string& message) {

}

void DRenderEngine::RenderText(std::string text, float x, float y, float scale) {
    textshader->Use();
    glUniform3f(glGetUniformLocation(textshader->ID, "textColor"), 1.0f, 1.0f, 0.5f);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}