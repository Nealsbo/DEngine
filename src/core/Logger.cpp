#include "Logger.h"

#include <cstdio>

unsigned int Log::outputMode = 1;
unsigned int Log::currLogLevel = LOG_LEVEL::INFO;

const std::string log_PreString[] = {
    "VERB : ",
    "INFO : ",
    "DEBUG: ",
    "WARN : ",
    "ERROR: "
};

void Log::Msg(const std::string& message, const LOG_LEVEL level) {
    if (outputMode && currLogLevel >= level)
        printMsg(message, level);
}

void Log::SetLevel(enum LOG_LEVEL newLevel) {
    currLogLevel = newLevel;
}

void Log::EnableLogger(bool enable) {
    outputMode = (unsigned int)enable;
}

void Log::printMsg(const std::string& message, const LOG_LEVEL level) {
    printf("%s%s\n", log_PreString[level].c_str(), message.c_str());
}