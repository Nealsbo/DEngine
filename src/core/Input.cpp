#include "Input.h"

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
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
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
                SDL_GetMouseState( &mouseXPos, &mouseYPos );
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
    return mouseyOffset;
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
        //if(keys[i].pressed)
        printf("key: %s| state: %i\n", keynames[i].name.c_str(), keys[i].pressed);
    }
    printf("mouse|X: %i, Y: %i|\n", mouseXPos, mouseYPos);
    printf("|-----|\n");
}

int keymap(int key) {
    switch(key) {
    case SDLK_w:
        return KEY_W;
    case SDLK_a:
        return KEY_A;
    case SDLK_s:
        return KEY_S;
    case SDLK_d:
        return KEY_D;
    case SDLK_SPACE:
        return KEY_SPACE;
    case SDLK_TAB:
        return KEY_TAB;
    case SDLK_LCTRL:
        return KEY_CTRL;
    case SDLK_ESCAPE:
        return KEY_ESC;
    default:
        return KEY_ESC;
    }
}