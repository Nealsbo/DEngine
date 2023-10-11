#pragma once

#include <cstdint>
#include <vector>
#include <string>

class DRenderEngine;

class DSession {
public:
    DSession();
    ~DSession();

    int Init();
    void Shutdown();

    void Frame();
    void UpdateCmdBuffer();
    void Render();
    void ProcessEvent();

private:
    DRenderEngine *renderer;
};
