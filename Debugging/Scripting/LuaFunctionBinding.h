// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __LUAFUNCTIONBINDING_H__
#define __LUAFUNCTIONBINDING_H__

#include "HumaNature/Scripting/LuaUtil.h"

namespace core {

    class LuaFunctionBinding
        : public MemoryObject
        , public LuaUtil
    {
    public:
        // TODO - remove for release?
        std::string mName;

        virtual ~LuaFunctionBinding();

        virtual void call(lua_State* L);

    protected:
        template<typename ParameterType>
        ParameterType interpretLuaParameter(const char* forFunction, lua_State* L, int parameterIndex);

        template<typename ReturnType>
        int pushReturnValue(lua_State* L, const ReturnType& returnValue);

        virtual void intepretLuaParametersAndCall(lua_State* L) = 0;
    };

} // namespace core

#endif // __LUAFUNCTIONBINDING_H__