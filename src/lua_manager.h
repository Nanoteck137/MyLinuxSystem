#pragma once

/*
  TODO(patrik):
    - GetGlobal/SetGlobal
    - Types
    - Tables
    - Register Custom Libraries
 */

class LuaManager
{
private:
    lua_State* m_State;
public:
    LuaManager();
    ~LuaManager();

    void RunScript(const std::string& script);
    
    void SetGlobal(const std::string& name, LuaValue* value);
};
