// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __LuaGlobalProcedureBindingS_H__
#define __LuaGlobalProcedureBindingS_H__

#include "LuaFunctionBinding.h"

namespace core {

    template<typename ReturnType>
    class LuaGlobalProcedureBinding0
        : public LuaFunctionBinding
    {
    public:
        LuaGlobalProcedureBinding0(ReturnType (*globalFunction)())
            : mGlobalFunction(globalFunction)
        {

        };

        virtual void interpretLuaParametersAndCall(lua_State* L)
        {
            UNUSED_PARAM(L);

            (*mGlobalFunction)();
        }

    protected:
        
        ReturnType (*mGlobalFunction)();
    };

    template<typename ReturnType, typename ParamType0>
    class LuaGlobalProcedureBinding1
        : public LuaFunctionBinding
    {
    public:
        LuaGlobalProcedureBinding1(ReturnType (*globalFunction)(ParamType0))
            : mGlobalFunction(globalFunction)
        {

        };

        virtual void interpretLuaParametersAndCall(lua_State* L)
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
    class LuaGlobalProcedureBinding2
        : public LuaFunctionBinding
    {
    public:
        LuaGlobalProcedureBinding2(ReturnType (*globalFunction)(ParamType0, ParamType1))
            : mGlobalFunction(globalFunction)
        {

        };

        virtual void interpretLuaParametersAndCall(lua_State* L)
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
    class LuaGlobalProcedureBinding3
        : public LuaFunctionBinding
    {
    public:
        LuaGlobalProcedureBinding3(ReturnType (*globalFunction)(ParamType0, ParamType1, ParamType2))
            : mGlobalFunction(globalFunction)
        {

        };

        virtual void interpretLuaParametersAndCall(lua_State* L)
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
    class LuaGlobalProcedureBinding4
        : public LuaFunctionBinding
    {
    public:
        LuaGlobalProcedureBinding4(ReturnType (*globalFunction)(ParamType0, ParamType1, ParamType2, ParamType3))
            : mGlobalFunction(globalFunction)
        {

        };

        virtual void interpretLuaParametersAndCall(lua_State* L)
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
    class LuaGlobalProcedureBinding5
        : public LuaFunctionBinding
    {
    public:
        LuaGlobalProcedureBinding5(ReturnType (*globalFunction)(ParamType0, ParamType1, ParamType2, ParamType3, ParamType4))
            : mGlobalFunction(globalFunction)
        {

        };

        virtual void interpretLuaParametersAndCall(lua_State* L)
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

#endif // __LuaGlobalProcedureBindingS_H__