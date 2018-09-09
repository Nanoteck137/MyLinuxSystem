#pragma once

#include <string>
#include <assert.h>

#include <lua.hpp>

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
    
    template<typename T>
    void SetGlobal(const std::string& name, T value)
    {
        assert(false);
    }
};

template<>
void LuaManager::SetGlobal<lua_Number>(const std::string& name, lua_Number value)
{
    lua_pushnumber(m_State, value);
    lua_setglobal(m_State, name.c_str());
}
