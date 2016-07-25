// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __LUAGLOBALFUNCTIONBINDINGS_H__
#define __LUAGLOBALFUNCTIONBINDINGS_H__

#include "LuaFunctionBinding.h"

namespace core {

    template<typename ReturnType>
    class LuaGlobalFunctionBinding0
        : public LuaFunctionBinding
    {
    public:
        LuaGlobalFunctionBinding0(ReturnType (*globalFunction)())
            : mGlobalFunction(globalFunction)
        {

        };

        virtual void intepretLuaParametersAndCall(lua_State* L)
        {
            UNUSED_PARAM(L);

            (*mGlobalFunction)();
        }

    protected:
        
        ReturnType (*mGlobalFunction)();
    };

    template<typename ReturnType, typename ParamType0>
    class LuaGlobalFunctionBinding1
        : public LuaFunctionBinding
    {
    public:
        LuaGlobalFunctionBinding1(ReturnType (*globalFunction)(ParamType0))
            : mGlobalFunction(globalFunction)
        {

        };

        virtual void intepretLuaParametersAndCall(lua_State* L)
        {
            (*mGlobalFunction)
                (
                interpretLuaParameter<typename StripConstReference<ParamType0>::Type>(mName.c_str(), L, 0)
                );
        }

    protected:
        ReturnType (*mGlobalFunction)(ParamType0);
    };

    template <typename ReturnType, typename ParamType0, typename ParamType1>
    class LuaGlobalFunctionBinding2
        : public LuaFunctionBinding
    {
    public:
        LuaGlobalFunctionBinding2(ReturnType (*globalFunction)(ParamType0, ParamType1))
            : mGlobalFunction(globalFunction)
        {

        };

        virtual void intepretLuaParametersAndCall(lua_State* L)
        {
            (*mGlobalFunction)
                (
                interpretLuaParameter<typename StripConstReference<ParamType0>::Type>(mName.c_str(), L, 0), 
                interpretLuaParameter<typename StripConstReference<ParamType1>::Type>(mName.c_str(), L, 1)
                );
        }

    protected:
        ReturnType (*mGlobalFunction)(ParamType0, ParamType1);
    };

    template <typename ReturnType, typename ParamType0, typename ParamType1, typename ParamType2>
    class LuaGlobalFunctionBinding3
        : public LuaFunctionBinding
    {
    public:
        LuaGlobalFunctionBinding3(ReturnType (*globalFunction)(ParamType0, ParamType1, ParamType2))
            : mGlobalFunction(globalFunction)
        {

        };

        virtual void intepretLuaParametersAndCall(lua_State* L)
        {
            (*mGlobalFunction)
                (
                interpretLuaParameter<typename StripConstReference<ParamType0>::Type>(mName.c_str(), L, 0), 
                interpretLuaParameter<typename StripConstReference<ParamType1>::Type>(mName.c_str(), L, 1),
                interpretLuaParameter<typename StripConstReference<ParamType2>::Type>(mName.c_str(), L, 2)
                );
        }

    protected:
        ReturnType (*mGlobalFunction)(ParamType0, ParamType1, ParamType2);
    };

    template <typename ReturnType, typename ParamType0, typename ParamType1, typename ParamType2, typename ParamType3>
    class LuaGlobalFunctionBinding4
        : public LuaFunctionBinding
    {
    public:
        LuaGlobalFunctionBinding4(ReturnType (*globalFunction)(ParamType0, ParamType1, ParamType2, ParamType3))
            : mGlobalFunction(globalFunction)
        {

        };

        virtual void intepretLuaParametersAndCall(lua_State* L)
        {
            (*mGlobalFunction)
                (
                interpretLuaParameter<typename StripConstReference<ParamType0>::Type>(mName.c_str(), L, 0), 
                interpretLuaParameter<typename StripConstReference<ParamType1>::Type>(mName.c_str(), L, 1),
                interpretLuaParameter<typename StripConstReference<ParamType2>::Type>(mName.c_str(), L, 2),
                interpretLuaParameter<typename StripConstReference<ParamType3>::Type>(mName.c_str(), L, 3)
                );
        }

    protected:
        ReturnType (*mGlobalFunction)(ParamType0, ParamType1, ParamType2, ParamType3);
    };

    template <typename ReturnType, typename ParamType0, typename ParamType1, typename ParamType2, typename ParamType3, typename ParamType4>
    class LuaGlobalFunctionBinding5
        : public LuaFunctionBinding
    {
    public:
        LuaGlobalFunctionBinding5(ReturnType (*globalFunction)(ParamType0, ParamType1, ParamType2, ParamType3, ParamType4))
            : mGlobalFunction(globalFunction)
        {

        };

        virtual void intepretLuaParametersAndCall(lua_State* L)
        {
            (*mGlobalFunction)
                (
                interpretLuaParameter<typename StripConstReference<ParamType0>::Type>(mName.c_str(), L, 0), 
                interpretLuaParameter<typename StripConstReference<ParamType1>::Type>(mName.c_str(), L, 1),
                interpretLuaParameter<typename StripConstReference<ParamType2>::Type>(mName.c_str(), L, 2),
                interpretLuaParameter<typename StripConstReference<ParamType3>::Type>(mName.c_str(), L, 3),
                interpretLuaParameter<typename StripConstReference<ParamType4>::Type>(mName.c_str(), L, 4)
                );
        }

    protected:
        ReturnType (*mGlobalFunction)(ParamType0, ParamType1, ParamType2, ParamType3, ParamType4);
    };

} // namespace core

#endif // __LUAGLOBALFUNCTIONBINDINGS_H__