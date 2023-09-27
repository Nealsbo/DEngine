#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Texture.h"

struct QuadParams_t {
    glm::vec2  pos;
    glm::vec2  size;
    glm::vec2  uvOffset;
    glm::vec3  color;
    float      angle;
    DTexture  *texture;
    Shader    *shader;
};

class DQuadRenderer {
    public:
        DQuadRenderer();
        ~DQuadRenderer();

        void SetParams(QuadParams_t *p);
        QuadParams_t& GetParams();

        void SetShader(Shader *s);
        void SetTexture(DTexture *t);

        void Draw(QuadParams_t *p);

    private:
        void InitRenderData();
        void UpdateRenderData();

        QuadParams_t  params;
        unsigned int  quadVAO;
        glm::mat4     model   = glm::mat4(1.0f);
};