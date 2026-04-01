#include "CVar.h"

DCVar::DCVar(const char* name, const char* value, DCVarValueType& type, const char* desc) {
    cvar_name = name;
    cvar_desc = desc;
    switch (type)
    {
    case DCVarValueType::CVAR_BOOL:
        /* code */
        bvalue = std::string(value).compare("true") == 0;
        break;
    
    case DCVarValueType::CVAR_FLOAT:
        ivalue = std::stof(value);
        break;
    
    case DCVarValueType::CVAR_INT:
        ivalue = std::stoi(value);
        break;
    
    case DCVarValueType::CVAR_STRING:
        svalue = std::string(value);
        break;

    default:
        break;
    }
}

void DCVar::Init(const char* name, const char* value, DCVarValueType& type, const char* desc) {
    
}