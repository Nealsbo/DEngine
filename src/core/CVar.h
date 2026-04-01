#pragma once

#include <string>
#include <unordered_map>

enum class DCVarValueType {
    NONE = 0,
    CVAR_FLOAT,
    CVAR_INT,
    CVAR_BOOL,
    CVAR_STRING
};

struct DCVar {
    DCVar(const char* name, const char* value, DCVarValueType& type, const char* desc);

    void Init(const char* name, const char* value, DCVarValueType& type, const char* desc);
    void Register();

    std::string GetDescription();
    
    void GetValueF();

    std::string cvar_name;
    std::string cvar_desc;
    std::string svalue;
    
    float fvalue;
    float min_fvalue;
    float max_fvalue;
    
    int ivalue;
    int max_ivalue;
    int min_ivalue;
    
    bool bvalue;

    std::string string_value;
};

class DCVarSystem {
public:
    virtual void Init();
    virtual void Shutdown();

    virtual void Register(DCVar* cvar);
    
    static DCVarSystem* Get() {
        static DCVarSystem cvar_sys;
        return &cvar_sys;
    }

};