#pragma once

#include "RendererBase.h"

class RendererOpenGL : public RenderBase {
public:
    RendererOpenGL();
    ~RendererOpenGL();

    void Init() override;
    void Shutdown() override;

    void BeginFrame() override;
    void EndFrame() override;

    void SetPlayerView(const DCamera& cam) override;
    void RenderFrame() override;

    float frameTime;
    int frameCount;

    DCamera view;
};