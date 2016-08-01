// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __CORELUASCRIPT_H__
#define __CORELUASCRIPT_H__

#include "HumaNature/Scripting/LuaScript.h"

namespace core {

    // Ties LuaScript into systems unavailable in PhyreCore library,
    // such as the SLED debugger and profiling
    class CoreLuaScript
        : public LuaScript
    {
    public:
        CoreLuaScript(lua_State* preAllocatedState = nullptr);
        virtual ~CoreLuaScript();

    protected:
        virtual bool executeScriptString(const char *script, const char *scriptPath = NULL);
        virtual bool parseProperties();
    };

} // namespace core

#endif // __CORELUASCRIPT_H__
