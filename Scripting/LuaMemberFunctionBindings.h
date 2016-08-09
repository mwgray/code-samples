// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __LUAMEMBERFUNCTIONBINDINGS_H__
#define __LUAMEMBERFUNCTIONBINDINGS_H__

#include "LuaFunctionBinding.h"

namespace core {

    template <typename ObjectClass, typename ReturnType>
    class LuaMemberFunctionBinding0
        : public LuaFunctionBinding
    {
    public:
        LuaMemberFunctionBinding0(ObjectClass* object, ReturnType (ObjectClass::*memberFunction)())
            : mObject(object)
            , mMemberFunction(memberFunction)
        {

        };

        virtual void interpretLuaParametersAndCall(lua_State* L)
        {
            UNUSED_PARAM(L);

            ReturnType returnValue = (*mObject.*mMemberFunction)();

            pushReturnValue<typename StripConstReference<ReturnType>::Type>(L, returnValue);
        }

    protected:
        ObjectClass* mObject;
        ReturnType (ObjectClass::*mMemberFunction)();
    };

    template <typename ObjectClass, typename ParamType0, typename ReturnType>
    class LuaMemberFunctionBinding1
        : public LuaFunctionBinding
    {
    public:
        LuaMemberFunctionBinding1(ObjectClass* object, ReturnType (ObjectClass::*memberFunction)(ParamType0))
            : mObject(object)
            , mMemberFunction(memberFunction)
        {

        };

        virtual void interpretLuaParametersAndCall(lua_State* L)
        {
            ReturnType returnValue = (*mObject.*mMemberFunction)
                (
                    interpretLuaParameter<typename StripConstReference<ParamType0>::Type>(mName.c_str(), L, 0)
                );

            pushReturnValue<typename StripConstReference<ReturnType>::Type>(L, returnValue);
        }

    protected:
        ObjectClass* mObject;
        ReturnType (ObjectClass::*mMemberFunction)(ParamType0);
    };

    template <typename ObjectClass, typename ParamType0, typename ParamType1, typename ReturnType>
    class LuaMemberFunctionBinding2
        : public LuaFunctionBinding
    {
    public:
        LuaMemberFunctionBinding2(ObjectClass* object, ReturnType (ObjectClass::*memberFunction)(ParamType0, ParamType1))
            : mObject(object)
            , mMemberFunction(memberFunction)
        {

        };

        virtual void interpretLuaParametersAndCall(lua_State* L)
        {
            ReturnType returnValue = (*mObject.*mMemberFunction)
                (
                    interpretLuaParameter<typename StripConstReference<ParamType0>::Type>(mName.c_str(), L, 0), 
                    interpretLuaParameter<typename StripConstReference<ParamType1>::Type>(mName.c_str(), L, 1)
                );

            pushReturnValue<typename StripConstReference<ReturnType>::Type>(L, returnValue);
        }

    protected:
        ObjectClass* mObject;
        ReturnType (ObjectClass::*mMemberFunction)(ParamType0, ParamType1);
    };

    template <typename ObjectClass, typename ParamType0, typename ParamType1, typename ParamType2, typename ReturnType>
    class LuaMemberFunctionBinding3
        : public LuaFunctionBinding
    {
    public:
        LuaMemberFunctionBinding3(ObjectClass* object, ReturnType (ObjectClass::*memberFunction)(ParamType0, ParamType1, ParamType2))
            : mObject(object)
            , mMemberFunction(memberFunction)
        {

        };

        virtual void interpretLuaParametersAndCall(lua_State* L)
        {
            ReturnType returnValue = (*mObject.*mMemberFunction)
                (
                interpretLuaParameter<typename StripConstReference<ParamType0>::Type>(mName.c_str(), L, 0), 
                interpretLuaParameter<typename StripConstReference<ParamType1>::Type>(mName.c_str(), L, 1),
                interpretLuaParameter<typename StripConstReference<ParamType2>::Type>(mName.c_str(), L, 2)
                );

            pushReturnValue<typename StripConstReference<ReturnType>::Type>(L, returnValue);
        }

    protected:
        ObjectClass* mObject;
        ReturnType (ObjectClass::*mMemberFunction)(ParamType0, ParamType1, ParamType2);
    };

    template <typename ObjectClass, typename ParamType0, typename ParamType1, typename ParamType2, typename ParamType3, typename ReturnType>
    class LuaMemberFunctionBinding4
        : public LuaFunctionBinding
    {
    public:
        LuaMemberFunctionBinding4(ObjectClass* object, ReturnType (ObjectClass::*memberFunction)(ParamType0, ParamType1, ParamType2, ParamType3))
            : mObject(object)
            , mMemberFunction(memberFunction)
        {

        };

        virtual void interpretLuaParametersAndCall(lua_State* L)
        {
            ReturnType returnValue = (*mObject.*mMemberFunction)
                (
                interpretLuaParameter<typename StripConstReference<ParamType0>::Type>(mName.c_str(), L, 0), 
                interpretLuaParameter<typename StripConstReference<ParamType1>::Type>(mName.c_str(), L, 1),
                interpretLuaParameter<typename StripConstReference<ParamType2>::Type>(mName.c_str(), L, 2),
                interpretLuaParameter<typename StripConstReference<ParamType3>::Type>(mName.c_str(), L, 3)
                );

            pushReturnValue<typename StripConstReference<ReturnType>::Type>(L, returnValue);
        }

    protected:
        ObjectClass* mObject;        
        ReturnType (ObjectClass::*mMemberFunction)(ParamType0, ParamType1, ParamType2, ParamType3);
    };

    template <typename ObjectClass, typename ParamType0, typename ParamType1, typename ParamType2, typename ParamType3, typename ParamType4, typename ReturnType>
    class LuaMemberFunctionBinding5
        : public LuaFunctionBinding
    {
    public:
        LuaMemberFunctionBinding5(ObjectClass* object, ReturnType (ObjectClass::*memberFunction)(ParamType0, ParamType1, ParamType2, ParamType3, ParamType4))
            : mObject(object)
            , mMemberFunction(memberFunction)
        {

        };

        virtual void interpretLuaParametersAndCall(lua_State* L)
        {
            ReturnType returnValue = (*mObject.*mMemberFunction)
                (
                interpretLuaParameter<typename StripConstReference<ParamType0>::Type>(mName.c_str(), L, 0), 
                interpretLuaParameter<typename StripConstReference<ParamType1>::Type>(mName.c_str(), L, 1),
                interpretLuaParameter<typename StripConstReference<ParamType2>::Type>(mName.c_str(), L, 2),
                interpretLuaParameter<typename StripConstReference<ParamType3>::Type>(mName.c_str(), L, 3),
                interpretLuaParameter<typename StripConstReference<ParamType4>::Type>(mName.c_str(), L, 4)
                );

            pushReturnValue<typename StripConstReference<ReturnType>::Type>(L, returnValue);
        }

    protected:
        ObjectClass* mObject;
        ReturnType (ObjectClass::*mMemberFunction)(ParamType0, ParamType1, ParamType2, ParamType3, ParamType4);
    };

} // namespace core

#include "LuaMemberProcedureBindings.h"

#endif // __LUAMEMBERFUNCTIONBINDINGS_H__