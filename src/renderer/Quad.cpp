#include "Quad.h"

static const char * quadVertexShader = 
"#version 400 core\n"
"layout (location = 0) in vec4 vertex;\n"
"out vec2 TexCoords;\n"
"uniform mat4 model;\n"
"uniform mat4 projection;\n"
"void main() {\n"
"    TexCoords = vertex.zw;\n"
"    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);\n"
"}\0";

static const char * quadFragmentShader = 
"#version 400 core\n"
"in vec2 TexCoords\n"
"out vec4 color;\n"
"uniform vec3 s_color;\n"
"uniform float s_opacity;\n"
"void main() {\n"
"    color = vec4(s_color, s_opacity);\n"
"}\0";

static float quadVertices[] = {
    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 
    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f
    };

static unsigned int sVAO;

DQuadRenderer::DQuadRenderer() {}
DQuadRenderer::~DQuadRenderer() {}

void DQuadRenderer::SetParams(QuadParams_t *p) {
    params.pos = p->pos;
    params.size = p->size;
    params.uvOffset = p->uvOffset;
    params.color = p->color;
    params.angle = p->angle;
    params.texture = p->texture;
    params.shader = p->shader;

    UpdateRenderData();
}

void DQuadRenderer::SetShader(Shader *s) {
    params.shader = s;
}

void DQuadRenderer::SetTexture(DTexture *t) {
    params.texture = t;
}


QuadParams_t& DQuadRenderer::GetParams() {
    return params;
}

void DQuadRenderer::Draw(QuadParams_t *p) {
    InitRenderData();

    params.shader->Use();

    params.shader->SetMat4("model", model);
    params.shader->SetVec3("color", color);

    glActiveTexture(GL_TEXTURE0);
    params.texture->Bind();

    glBindVertexArray(sVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void DQuadRenderer::InitRenderData() {
    unsigned int VBO;

    glGenVertexArrays(1, &sVAO);
    glGenBuffers(1, &VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glBindVertexArray(sVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  
    glBindVertexArray(0);
}

void DQuadRenderer::UpdateRenderData() {
    model = glm::translate(model, glm::vec3(params.pos, 0.0f));
    model = glm::translate(model, glm::vec3(0.5f * params.size.x, 0.5f * params.size.y, 0.0f));
    model = glm::rotate(model, glm::radians(params.angle), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * params.size.x, -0.5f * params.size.y, 0.0f));
    model = glm::scale(model, glm::vec3(params.size, 1.0f));
}