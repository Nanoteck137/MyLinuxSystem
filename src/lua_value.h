#pragma once

class LuaValue
{
private:
    enum class Type
    {
        TYPE_UNKNOWN,
        TYPE_NUMBER,
        TYPE_STRING,
        NUM_TYPES,
    };
    
    union Data
    {
        lua_Number number;
        char* string;
    };
private:
    Data m_Data;
    Type m_Type;
private:
    LuaValue(Type type);
public:
    ~LuaValue();

    void Push(lua_State* state);
public:
    static LuaValue* From(int value);
    static LuaValue* From(float value);
    static LuaValue* From(double value);
    static LuaValue* From(const std::string& value);
};
