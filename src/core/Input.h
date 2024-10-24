#ifndef _INPUT_H_
#define _INPUT_H_

#include <SDL2/SDL.h>

#include <string>

enum {
    KEY_W = 1,
    KEY_A,
    KEY_S,
    KEY_D,
    KEY_SPACE,
    KEY_TAB,
    KEY_ESC,
    KEY_CTRL,
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