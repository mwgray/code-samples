// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __LUAMEMBERPROCEDUREBINDINGS_H__
#define __LUAMEMBERPROCEDUREBINDINGS_H__

#include "HumaNature/Templates/TypeTraits.h"

namespace core {

    template <typename ObjectClass>
    class LuaMemberFunctionBinding0<ObjectClass, void>
        : public LuaFunctionBinding
    {
    public:
        LuaMemberFunctionBinding0(ObjectClass* object, void (ObjectClass::*memberFunction)())
            : mObject(object)
            , mMemberFunction(memberFunction)
        {

        };

        virtual void interpretLuaParametersAndCall(lua_State* L)
        {
            UNUSED_PARAM(L);

            (*mObject.*mMemberFunction)();
        }

    protected:
        ObjectClass* mObject;
        void (ObjectClass::*mMemberFunction)();
    };

    template <typename ObjectClass, typename ParamType0>
    class LuaMemberFunctionBinding1<ObjectClass, ParamType0, void>
        : public LuaFunctionBinding
    {
    public:
        LuaMemberFunctionBinding1(ObjectClass* object, void (ObjectClass::*memberFunction)(ParamType0))
            : mObject(object)
            , mMemberFunction(memberFunction)
        {

        };

        virtual void interpretLuaParametersAndCall(lua_State* L)
        {
            (*mObject.*mMemberFunction)
                (
                    interpretLuaParameter< typename StripConstReference<ParamType0>::Type >(mName.c_str(), L, 0)
                );
        }

    protected:
        ObjectClass* mObject;
        void (ObjectClass::*mMemberFunction)(ParamType0);
    };

    template <typename ObjectClass, typename ParamType0, typename ParamType1>
    class LuaMemberFunctionBinding2<ObjectClass, ParamType0, ParamType1, void>
        : public LuaFunctionBinding
    {
    public:
        LuaMemberFunctionBinding2(ObjectClass* object, void (ObjectClass::*memberFunction)(ParamType0, ParamType1))
            : mObject(object)
            , mMemberFunction(memberFunction)
        {

        };

        virtual void interpretLuaParametersAndCall(lua_State* L)
        {
            (*mObject.*mMemberFunction)
                (
                    interpretLuaParameter<typename StripConstReference<ParamType0>::Type>(mName.c_str(), L, 0), 
                    interpretLuaParameter<typename StripConstReference<ParamType1>::Type>(mName.c_str(), L, 1)
                );
        }

    protected:
        ObjectClass* mObject;
        void (ObjectClass::*mMemberFunction)(ParamType0, ParamType1);
    };

    template <typename ObjectClass, typename ParamType0, typename ParamType1, typename ParamType2>
    class LuaMemberFunctionBinding3<ObjectClass, ParamType0, ParamType1, ParamType2, void>
        : public LuaFunctionBinding
    {
    public:
        LuaMemberFunctionBinding3(ObjectClass* object, void (ObjectClass::*memberFunction)(ParamType0, ParamType1, ParamType2))
            : mObject(object)
            , mMemberFunction(memberFunction)
        {

        };

        virtual void interpretLuaParametersAndCall(lua_State* L)
        {
            (*mObject.*mMemberFunction)
                (
                interpretLuaParameter<typename StripConstReference<ParamType0>::Type>(mName.c_str(), L, 0), 
                interpretLuaParameter<typename StripConstReference<ParamType1>::Type>(mName.c_str(), L, 1),
                interpretLuaParameter<typename StripConstReference<ParamType2>::Type>(mName.c_str(), L, 2)
                );
        }

    protected:
        ObjectClass* mObject;
        void (ObjectClass::*mMemberFunction)(ParamType0, ParamType1, ParamType2);
    };

    template <typename ObjectClass, typename ParamType0, typename ParamType1, typename ParamType2, typename ParamType3>
    class LuaMemberFunctionBinding4<ObjectClass, ParamType0, ParamType1, ParamType2, ParamType3, void>
        : public LuaFunctionBinding
    {
    public:
        LuaMemberFunctionBinding4(ObjectClass* object, void (ObjectClass::*memberFunction)(ParamType0, ParamType1, ParamType2, ParamType3))
            : mObject(object)
            , mMemberFunction(memberFunction)
        {

        };

        virtual void interpretLuaParametersAndCall(lua_State* L)
        {
            (*mObject.*mMemberFunction)
                (
                interpretLuaParameter<typename StripConstReference<ParamType0>::Type>(mName.c_str(), L, 0), 
                interpretLuaParameter<typename StripConstReference<ParamType1>::Type>(mName.c_str(), L, 1),
                interpretLuaParameter<typename StripConstReference<ParamType2>::Type>(mName.c_str(), L, 2),
                interpretLuaParameter<typename StripConstReference<ParamType3>::Type>(mName.c_str(), L, 3)
                );
        }

    protected:
        ObjectClass* mObject;        
        void (ObjectClass::*mMemberFunction)(ParamType0, ParamType1, ParamType2, ParamType3);
    };

    template <typename ObjectClass, typename ParamType0, typename ParamType1, typename ParamType2, typename ParamType3, typename ParamType4>
    class LuaMemberFunctionBinding5<ObjectClass, ParamType0, ParamType1, ParamType2, ParamType3, ParamType4, void>
        : public LuaFunctionBinding
    {
    public:
        LuaMemberFunctionBinding5(ObjectClass* object, void (ObjectClass::*memberFunction)(ParamType0, ParamType1, ParamType2, ParamType3, ParamType4))
            : mObject(object)
            , mMemberFunction(memberFunction)
        {

        };

        virtual void interpretLuaParametersAndCall(lua_State* L)
        {
            (*mObject.*mMemberFunction)
                (
                interpretLuaParameter<typename StripConstReference<ParamType0>::Type>(mName.c_str(), L, 0), 
                interpretLuaParameter<typename StripConstReference<ParamType1>::Type>(mName.c_str(), L, 1),
                interpretLuaParameter<typename StripConstReference<ParamType2>::Type>(mName.c_str(), L, 2),
                interpretLuaParameter<typename StripConstReference<ParamType3>::Type>(mName.c_str(), L, 3),
                interpretLuaParameter<typename StripConstReference<ParamType4>::Type>(mName.c_str(), L, 4)
                );
        }

    protected:
        ObjectClass* mObject;
        void (ObjectClass::*mMemberFunction)(ParamType0, ParamType1, ParamType2, ParamType3, ParamType4);
    };

} // namespace core

#endif // __LUAMEMBERPROCEDUREBINDINGS_H__