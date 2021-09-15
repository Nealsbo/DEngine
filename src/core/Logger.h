#pragma once

#include <string>

enum LOG_LEVEL {
    VERBOSE = 0,
    INFO    = 1,
    DEBUG   = 2,
    WARNING = 3,
    ERROR   = 4,
};

class Log {
public:
    static void Msg(const std::string& message, const LOG_LEVEL level);
    static void SetLevel(enum LOG_LEVEL newLevel);
    static void EnableLogger(bool enable);

private:
    static void printMsg(const std::string& message, const LOG_LEVEL level);
    static unsigned int outputMode;
    static unsigned int currLogLevel;
};