#include "lua_value.h"

LuaValue::LuaValue(Type type)
    : m_Type(type)
{
}

LuaValue::~LuaValue()
{
    if(m_Type == Type::TYPE_STRING)
    {
        delete[] m_Data.string;
    }
}

void LuaValue::Push(lua_State* state)
{
    switch(m_Type)
    {
        case Type::TYPE_NUMBER:
            lua_pushnumber(state, m_Data.number);
            break;
            
        case Type::TYPE_STRING:
            lua_pushstring(state, m_Data.string);
            break;

        default:
            Log::Error("Trying to push unknown value: %d\n", m_Type);
            break;
    }
}

LuaValue* LuaValue::From(int value)
{
    LuaValue* result = new LuaValue(Type::TYPE_NUMBER);
    result->m_Data.number = (lua_Number)value;
    
    return result;
}

LuaValue* LuaValue::From(float value)
{
    LuaValue* result = new LuaValue(Type::TYPE_NUMBER);
    result->m_Data.number = (lua_Number)value;
    
    return result;
}

LuaValue* LuaValue::From(double value)
{
    LuaValue* result = new LuaValue(Type::TYPE_NUMBER);
    result->m_Data.number = (lua_Number)value;
    
    return result;
}

LuaValue* LuaValue::From(const std::string& value)
{
    LuaValue* result = new LuaValue(Type::TYPE_STRING);

    int strLength = value.length();
    char* str = new char[strLength + 1];
    memcpy(str, &value[0], strLength);
    str[strLength] = 0;
    
    result->m_Data.string = str;

    return result;
}
