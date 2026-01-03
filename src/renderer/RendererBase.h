#pragma once

#include <glm/glm.hpp>

#include "Camera.h"

class RenderBase {
public:
    RenderBase() = default;
    virtual ~RenderBase() = default;

    virtual void Init() = 0;
    virtual void Shutdown() = 0;

    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;

    virtual void SetPlayerView(const DCamera& cam) = 0;
    virtual void RenderFrame() = 0;
};