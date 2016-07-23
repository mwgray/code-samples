// Copyright 2006-13 HumaNature Studios Inc.

#include "CorePch.h"

#include "CoreLuaScript.h"
#include "NewStateMachine/stmAndLua.h"

namespace core {

    CoreLuaScript::CoreLuaScript(lua_State* preAllocatedState /* = nullptr */)
        : LuaScript(preAllocatedState)
    {
        if (!mWasPreAllocated)
        {
            // Register this lua state for use in the SLED debugger
            stmAndLua::GetInstance().RegisterLuaState(L);
        }
    }

    CoreLuaScript::~CoreLuaScript()
    {
        if(L && !mWasPreAllocated)
        {
            // un-register this lua state with SLED debugger
            stmAndLua::GetInstance().UnregisterLuaState(L);
        }
    }

    bool CoreLuaScript::executeScriptString(const char *script, const char *scriptPath /* = NULL */)
    {
        PROFILE_ZONE_DESC("LuaScript::loadScript::executeScript");
        return LuaScript::executeScriptString(script, scriptPath);
    }

    bool CoreLuaScript::parseProperties()
    {
        PROFILE_ZONE_DESC("LuaScript::loadScript::parseProperties");
        return LuaScript::parseProperties();
    }

}