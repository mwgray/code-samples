// Copyright 2006-12 HumaNature Studios Inc.

#include "CorePch.h"
#include "LuaContext.h"

#include "LuaScriptManager.h"

namespace core {
    LuaContext::LuaContext(lua_State* preAllocatedState)
        : CoreLuaScript(preAllocatedState)
    {
    }

    LuaContext::~LuaContext()
    {
        for(auto iter = mBindings.begin(); iter != mBindings.end(); ++iter)
        {
            SafeDelete(iter->second);
        }
    }

    void LuaContext::call(const std::string& name)
    {
        BindingMap::iterator find = mBindings.find(name);

        if(find == mBindings.end())
        {
            logWarn("Lua Binding", "Could not find bound function '%s'", name.c_str());
        }
        else
        {
            find->second->call(L);
        }
    }

    void LuaContext::unbind(const std::string& name)
    {
        BindingMap::iterator find = mBindings.find(name);

        if(find == mBindings.end())
        {
            logWarn("Lua Binding", "Trying to unbind unknown function '%s'!", name.c_str());
        }
        else
        {
            SafeDelete(find->second);
            mBindings.erase(find);
        }
    }

    void LuaContext::setBinding(const std::string& name, LuaFunctionBinding* binding)
    {
        BindingMap::iterator find = mBindings.find(name);

        if(find == mBindings.end())
        {
            mBindings[name] = binding;
            binding->mName = name;

            if(name.find('.') != std::string::npos)
            {
                // there's a . in the name, expecting a scope assignment
                std::string scope = name.substr(0, name.find_last_of('.'));
                std::string functionName = name.substr(name.find_last_of('.') + 1);

                int depth = PushScope(L, scope);

                // store the scoped function name for later retrieval
                lua_pushstring(L, name.c_str());

                // store the this pointer in the userdata to cast back to an instance from the global callback
                lua_pushlightuserdata(L, this);

                // push the closure onto the stack
                lua_pushcclosure(L, luaEntryFunction, 2);

                // set the closure to the function name
                lua_setfield(L, -2, functionName.c_str());

                // pop the scope
                PopScope(L, depth);
            }
            else
            {
                // is a global
                lua_pushstring(L, name.c_str());
                lua_pushlightuserdata(L, this);
                lua_pushcclosure(L, luaEntryFunction, 2);
                lua_setglobal(L, name.c_str());
            }
        }
        else
        {
            logWarn("Lua Binding", "Binding function name '%s', twice!  Ignoring second bind", name.c_str());

            SafeDelete(binding);
        }
    }

    int LuaContext::luaEntryFunction(lua_State* L)
    {
        const char* functionName = lua_tostring(L, lua_upvalueindex(1));
        void* contextPointer = lua_touserdata(L, lua_upvalueindex(2));

        LuaContext* context = static_cast<LuaContext*>(contextPointer);
        context->call(functionName);

        return 1;
    }
} // namespace core