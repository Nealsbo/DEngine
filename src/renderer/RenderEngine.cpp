#include "RenderEngine.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/gtc/type_ptr.hpp>

#include "imgui_impl_opengl3.h"
#include "imgui.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

std::string err;
std::string warn;

std::map<GLchar, Character> Characters;

DRenderEngine::DRenderEngine() {}

DRenderEngine::~DRenderEngine() {}

int DRenderEngine::Init(DWindowManager * wm) {
    win = wm;

    //glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if(!LoadFont()) {
        printf("ERROR: Font load failed!\n");
    }

    return 0;
}

void DRenderEngine::Shutdown() {
    delete textShader;
}

void DRenderEngine::DrawFrame(DScene *scene, float delta) {
    BeginFrame();

    DCamera *mainCamera = scene->GetMainCamera();

    for(const auto& model: scene->scene_models) {
        model->Draw(mainCamera->GetViewMatrix(), mainCamera->GetPosition(), scene->point_lights[0]);
    }

    currentFrameDelta = (currentFrameDelta + 1) % deltaBufferSize;
    avgFrameDelta[currentFrameDelta] = delta;

    DrawAvgFps();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    win->SwapBuffers();

    EndFrame();
}

void DRenderEngine::BeginFrame() {
    //start timer
    glClearColor(0.3f, 0.0f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void DRenderEngine::EndFrame() {
    //stop timer
}

void DRenderEngine::DrawAvgFps() {
    float avgFps = 0.0f;

    for(int i = 0; i < deltaBufferSize; i++) {
        avgFps += avgFrameDelta[i];
    }
    avgFps = avgFps / deltaBufferSize;

    std::string fps_text = "FPS: " + std::to_string(1000.0f / avgFps);
    RenderText(fps_text, 16.0f, 720.0f - 16.0f - 16.0f, 1.0f);
}
   
bool DRenderEngine::LoadFont() {
    bool success = true;

    textShader = new DShader("../assets/shaders/text.vs", "../assets/shaders/text.fs");
    glm::mat4 projection = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f);
    textShader->Use();
    glUniformMatrix4fv(glGetUniformLocation(textShader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

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
        FT_Set_Pixel_Sizes(face, 0, 24);

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
    textShader->Use();
    glUniform3f(glGetUniformLocation(textShader->ID, "textColor"), 0.5f, 1.0f, 0.5f);
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