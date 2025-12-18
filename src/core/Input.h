#ifndef _INPUT_H_
#define _INPUT_H_

#include <SDL2/SDL.h>

#include <string>

enum {
    KEY_W = 1,
    KEY_A,
    KEY_S,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_LMB,
    KEY_RMB,
    KEY_MMB,
    KEY_MWHEEL_UP,
    KEY_MWHEEL_DOWN,
    KEY_SPACE,
    KEY_TAB,
    KEY_ESC,
    KEY_CTRL,
    KEY_SHIFT,
    KEY_LASTKEY
};

class DKey {
public:
    DKey() { pressed = false; bind = ""; }

    bool pressed = false;
    std::string bind = "";
};

class DInputHandler {
public:
    DInputHandler();
    ~DInputHandler();

    int Init(SDL_Window* window);
    void Shutdown();
    void Update();

    void SetKeyBinding(int key, std::string bind);
    std::string GetKeyBinding(int key);

    int GetMouseXOffset();
    int GetMouseYOffset();

    bool IsKeyDown(int key);

private:
    void UpdateMousePosition();

    void DebugPrintInput();

    SDL_Window * window = nullptr;

    int mousexOffset;
    int mouseyOffset;
};

int keymap(int key);

#endif