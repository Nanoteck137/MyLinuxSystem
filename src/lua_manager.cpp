#include "lua_manager.h"

LuaManager::LuaManager()
    : m_State(nullptr)
{
    m_State = luaL_newstate();
    //TODO(patrik): Dont open the Lua standard library by default
    luaL_openlibs(m_State);
}

LuaManager::~LuaManager()
{
    lua_close(m_State);
    m_State = nullptr;
}

void LuaManager::RunScript(const std::string& script)
{
    luaL_dostring(m_State, script.c_str());
}

void LuaManager::SetGlobal(const std::string& name, LuaValue* value)
{
    value->Push(m_State);
    lua_setglobal(m_State, name.c_str());
}
