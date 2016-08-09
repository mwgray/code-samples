// Copyright 2006-12 HumaNature Studios Inc.

#include "CorePch.h"

#include "LuaFunctionBinding.h"

namespace core {

    // specializations
    template<> int LuaFunctionBinding::pushReturnValue(lua_State* L, const bool& returnValue) { lua_pushboolean(L, returnValue); return 1; }
    template<> int LuaFunctionBinding::pushReturnValue(lua_State* L, const int& returnValue)  { lua_pushinteger(L, returnValue); return 1; }
    template<> int LuaFunctionBinding::pushReturnValue(lua_State* L, const unsigned int& returnValue) { lua_pushinteger(L, returnValue); return 1; }
    template<> int LuaFunctionBinding::pushReturnValue(lua_State* L, const float& returnValue) { lua_pushnumber(L, returnValue); return 1; }
    template<> int LuaFunctionBinding::pushReturnValue(lua_State* L, const std::string& returnValue) { lua_pushstring(L, returnValue.c_str()); return 1; }
    template<> int LuaFunctionBinding::pushReturnValue(lua_State* L, const char* const& returnValue) { lua_pushstring(L, returnValue); return 1; }

    // dummys for complex return types....  not sure the ideal way to set this up, as VS & SNC fight
    class SceneObject;
    template<> int LuaFunctionBinding::pushReturnValue(lua_State*, SceneObject* const&) { return 1; }

    LuaFunctionBinding::~LuaFunctionBinding()
    {

    }

    void LuaFunctionBinding::call(lua_State* L)
    {
        if(L)
        {
            lua_State* parameterContext = L;

            if(lua_isthread(L, -1))
            {
                // first parameter is thread, use it as the context instead
                parameterContext =  lua_tothread(L, -1);
            }
            
            interpretLuaParametersAndCall(parameterContext);
        }
    }

    template<> int LuaFunctionBinding::interpretLuaParameter(const char* forFunction, lua_State* L, int parameterIndex) 
    { 
        int i = 0;

        if(!lua_isnumber(L, parameterIndex + 1))
        {
            logWarn("Lua Binding", "Could not convert %s's parameter '%d' to int(it's a [%s]),  %d", forFunction, parameterIndex, TypeOf(L, parameterIndex + 1), i);
        }
        else
        {
            i = luaL_checkint(L, parameterIndex + 1); 
        }
        
        return i;
    }

    template<> float LuaFunctionBinding::interpretLuaParameter(const char* forFunction, lua_State* L, int parameterIndex) 
    {
        float f = 0.0f;
            
        if(!lua_isnumber(L, parameterIndex + 1))
        {
            logWarn("Lua Binding", "Could not convert %s's parameter '%d' to int(it's a [%s]),  %f", forFunction, parameterIndex, TypeOf(L, parameterIndex + 1), f);
        }
        else
        {
            f = luaL_checknumber(L, parameterIndex + 1);
        }

        return f;
    }

    template<> bool LuaFunctionBinding::interpretLuaParameter(const char* forFunction, lua_State* L, int parameterIndex) 
    {
        bool b = false;

        if(!lua_isboolean(L, parameterIndex + 1))
        {
            logWarn("Lua Binding", "Could not convert %s's parameter '%d' to bool(it's a [%s]),  %s", forFunction, parameterIndex, TypeOf(L, parameterIndex + 1), b ? "true" : "false");
        }
        else
        {
            b = lua_toboolean(L, parameterIndex + 1) != 0;
        }

        return b; 
    }

    template<> const char* LuaFunctionBinding::interpretLuaParameter(const char* forFunction, lua_State* L, int parameterIndex) 
    {
        const char* s = nullptr;
        
        if(!lua_isstring(L, parameterIndex + 1))
        {
            logWarn("Lua Binding", "Could not convert %s's parameter '%d' to string(it's a [%s]),  nullptr", forFunction, parameterIndex, TypeOf(L, parameterIndex + 1));
        }
        else
        {
            s = lua_tostring(L, parameterIndex + 1);
        }

        return s; 
    }

    template<> std::string LuaFunctionBinding::interpretLuaParameter(const char* forFunction, lua_State* L, int parameterIndex) 
    {
        const char* s = "";

        if(!lua_isstring(L, parameterIndex + 1))
        {
            logWarn("Lua Binding", "Could not convert %s's parameter '%d' to std::string(it's a [%s]),  \"\"", forFunction, parameterIndex, TypeOf(L, parameterIndex + 1));
        }
        else
        {
            s = lua_tostring(L, parameterIndex + 1);
        }

        return s; 
    }

    template<> CompactStringDebug LuaFunctionBinding::interpretLuaParameter(const char* forFunction, lua_State* L, int parameterIndex) 
    {
        const char* s = "";

        if(!lua_isstring(L, parameterIndex + 1))
        {
            logWarn("Lua Binding", "Could not convert %s's parameter '%d' to CompactString(it's a [%s]),  \"\"", forFunction, parameterIndex, TypeOf(L, parameterIndex + 1));
        }
        else
        {
            s = lua_tostring(L, parameterIndex + 1);
        }

        return s; 
    }

    template<> CompactStringRelease LuaFunctionBinding::interpretLuaParameter(const char* forFunction, lua_State* L, int parameterIndex) 
    {
        const char* s = "";

        if(!lua_isstring(L, parameterIndex + 1))
        {
            logWarn("Lua Binding", "Could not convert %s's parameter '%d' to CompactString(it's a [%s]),  \"\"", forFunction, parameterIndex, TypeOf(L, parameterIndex + 1));
        }
        else
        {
            s = lua_tostring(L, parameterIndex + 1);
        }

        return s; 
    }

} // namespace core