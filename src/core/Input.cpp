#include "Input.h"

#include "imgui_impl_sdl2.h"
#include <stdio.h>
#include <array>
#include <vector>



extern bool isOnExitRequest;

const int MAX_KEYS = KEY_LASTKEY + 1;
int mouseXPos;
int mouseYPos;
int lastXPos;
int lastYPos;

struct Keyname {
    int code;
    std::string name;
};

static const Keyname keynames[] = {
    {0,           "NULL"},
    {KEY_W,       "W"},
    {KEY_A,       "A"},
    {KEY_S,       "S"},
    {KEY_D,       "D"},
    {KEY_LMB,     "LMB"},
    {KEY_RMB,     "RMB"},
    {KEY_MMB,     "MMB"},
    {KEY_SPACE,   "SPACE"},
    {KEY_TAB,     "TAB"},
    {KEY_ESC,     "ESC"},
    {KEY_CTRL,    "CTRL"},
    {KEY_LASTKEY, "LATSKEY"},
};

//keymap
std::array<DKey, MAX_KEYS> keys; 

/*
=== Input Handler part ===
*/

DInputHandler::DInputHandler() {}

DInputHandler::~DInputHandler() {}

int DInputHandler::Init(SDL_Window * dwindow) {
    this->window = dwindow;
    return 0;
}

void DInputHandler::Shutdown() {

}

void DInputHandler::Update() {
    mousexOffset = 0;
    mouseyOffset = 0;

    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        
        switch(event.type) {
            case SDL_QUIT:
                isOnExitRequest = true;
                break;
            
            case SDL_KEYDOWN:
                keys[keymap(event.key.keysym.sym)].pressed = true;
                break;

            case SDL_KEYUP:
                keys[keymap(event.key.keysym.sym)].pressed = false;
                break;

            case SDL_MOUSEMOTION:
                mousexOffset = event.motion.xrel;
                mouseyOffset = event.motion.yrel;
                mouseXPos = event.motion.x;
                mouseYPos = event.motion.y;
                break;

            case SDL_MOUSEBUTTONDOWN:
                keys[keymap(event.button.button)].pressed = true;
                break;

            case SDL_MOUSEBUTTONUP:
                keys[keymap(event.button.button)].pressed = false;
                break;

            default:
                break;
        }
    }

    //DebugPrintInput();

    if(keys[KEY_ESC].pressed == true)
        isOnExitRequest = true;
}

void DInputHandler::SetKeyBinding(int key, std::string bind) {

}

std::string DInputHandler::GetKeyBinding(int key) {
    return "";
}

int DInputHandler::GetMouseXOffset() {
    return mousexOffset;
}
int DInputHandler::GetMouseYOffset() {
    return -mouseyOffset;
}

bool DInputHandler::IsKeyDown(int key) {
    return keys[key].pressed;
}

void DInputHandler::UpdateMousePosition() {
    mousexOffset = mouseXPos - lastXPos;
    mouseyOffset = mouseYPos - lastYPos;

    lastXPos = mouseXPos;
    lastYPos = mouseYPos;
}

void DInputHandler::DebugPrintInput() {
    for(unsigned int i = 1; i < keys.size(); i++) {
        if(keys[i].pressed)
        printf("key: %s| state: %i\n", keynames[i].name.c_str(), keys[i].pressed);
    }
    printf("mouse|X: %i, Y: %i|\n", mouseXPos, mouseYPos);
    //printf("|-----|\n");
}

int keymap(int key) {
    switch(key) {
    //main keys
    case SDLK_w:
        return KEY_W;
    case SDLK_a:
        return KEY_A;
    case SDLK_s:
        return KEY_S;
    case SDLK_d:
        return KEY_D;
    case SDLK_e:
        return KEY_E;
    case SDLK_f:
        return KEY_F;
    //num keys
    case SDLK_0:
        return KEY_0;
    case SDLK_1:
        return KEY_1;
    case SDLK_2:
        return KEY_2;
    case SDLK_3:
        return KEY_3;
    case SDLK_4:
        return KEY_4;
    case SDLK_5:
        return KEY_5;
    case SDLK_6:
        return KEY_6;
    case SDLK_7:
        return KEY_7;
    case SDLK_8:
        return KEY_8;
    case SDLK_9:
        return KEY_9;
    //mouse keys
    case SDL_BUTTON_LEFT:
        return KEY_LMB;
    case SDL_BUTTON_RIGHT:
        return KEY_RMB;
    case SDL_BUTTON_MIDDLE:
        return KEY_MMB;
    //util keys
    case SDLK_SPACE:
        return KEY_SPACE;
    case SDLK_TAB:
        return KEY_TAB;
    case SDLK_LCTRL:
        return KEY_CTRL;
    case SDLK_LSHIFT:
        return KEY_SHIFT;
    case SDLK_ESCAPE:
        return KEY_ESC;
    default:
        return KEY_ESC;
    }
}