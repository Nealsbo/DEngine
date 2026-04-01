#include "Console.h"

DConsole::DConsole() {

}

DConsole::~DConsole() {

}

int DConsole::Init() {
    return 0;
}

void DConsole::Shutdown() {

}

void DConsole::Open() {
    isActive = true;
}

void DConsole::Close() {
    isActive = false;
}

void DConsole::ScrollUp() {

}

void DConsole::ScrollDown() {

}

void DConsole::Clear() {

}

int DConsole::ApplyCmd() {
    return 0;
}


void DConsole::Draw() {
    if (isActive) {
        //ImGui::NewFrame();
        //ImGui::Begin("Console", &isActive);
        //ImGui::Text("Sample text");
        //if (ImGui::Button("Quit App")) isOnExitRequest = true;
        //ImGui::End();
    }
}

void DConsole::SaveHistory() {

}

void DConsole::LoadHistory() {

}

void DConsole::ChangeOpacity() {

}