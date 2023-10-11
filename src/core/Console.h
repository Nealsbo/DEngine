#pragma once

#include <cstdint>
#include <vector>
#include <string>

#define CONSOLE_TEXT_SIZE 262144 // 256Kb buffer for console text

class DConsole {
public:
    DConsole();
    ~DConsole();

    int Init();
    void Shutdown();

    void Open();
    void Close();
    void ScrollUp();
    void ScrollDown();
    void Clear();
    int  ApplyCmd();

    void Draw();

    void SaveHistory();
    void LoadHistory();

private:
    void ChangeOpacity();

    std::vector<std::string> consoleLog;
    uint32_t consoleLogSize    = 1024;

    bool isFocused = false;
    bool isActive  = false;
    bool isOpaque  = false;
    bool canInput  = false;

    float openSize;
    float fontSize;
    float opacity;

    char c_text[CONSOLE_TEXT_SIZE];
};

