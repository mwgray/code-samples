// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __LUAFUNCTIONCONTEXT_H__
#define __LUAFUNCTIONCONTEXT_H__

#include "CoreLuaScript.h"

#include "LuaGlobalProcedureBindings.h"
#include "LuaMemberFunctionBindings.h"

namespace core {

    class LuaContext
        : public CoreLuaScript        
    {
    public:
        LuaContext(lua_State* preAllocatedState = nullptr);

        virtual ~LuaContext();
        //////////////////////////////////////////////////////////////////////////
        ///  Global functions
        //////////////////////////////////////////////////////////////////////////
        template <typename ReturnType>
        void bindGlobal(const std::string& name, ReturnType (*globalFunction)())
        {
            setBinding(name, new LuaGlobalProcedureBinding0<ReturnType>(globalFunction));
        }

        template<typename ReturnType, typename P0>
        void bindGlobal(const std::string& name, ReturnType (*globalFunction)(P0))
        {
            setBinding(name, new LuaGlobalProcedureBinding1<ReturnType, P0>(globalFunction));
        }

        template<typename ReturnType, typename P0, typename P1>
        void bindGlobal(const std::string& name, ReturnType (*globalFunction)(P0, P1))
        {
            setBinding(name, new LuaGlobalProcedureBinding2<ReturnType, P0, P1>(globalFunction));
        }

        template<typename ReturnType, typename P0, typename P1, typename P2>
        void bindGlobal(const std::string& name, ReturnType (*globalFunction)(P0, P1, P2))
        {
            setBinding(name, new LuaGlobalProcedureBinding3<ReturnType, P0, P1, P2>(globalFunction));
        }

        template<typename ReturnType, typename P0, typename P1, typename P2, typename P3>
        void bindGlobal(const std::string& name, ReturnType (*globalFunction)(P0, P1, P2, P3))
        {
            setBinding(name, new LuaGlobalProcedureBinding4<ReturnType, P0, P1, P2, P3>(globalFunction));
        }

        template<typename ReturnType, typename P0, typename P1, typename P2, typename P3, typename P4>
        void bindGlobal(const std::string& name, ReturnType (*globalFunction)(P0, P1, P2, P3, P4))
        {
            setBinding(name, new LuaGlobalProcedureBinding5<ReturnType, P0, P1, P2, P3, P4>(globalFunction));
        }
        
        //////////////////////////////////////////////////////////////////////////
        ///  Member functions
        //////////////////////////////////////////////////////////////////////////

        // Object = type of the pointer
        // FunctionClass = type of the function container, since this can be a base class, we need 
        //                 to clarify the template parameter
        template<typename Object, typename FunctionClass, typename ReturnType>
        void bind(const std::string& name, Object* object, ReturnType (FunctionClass::*memberFunction)())
        {
            setBinding(name, new LuaMemberFunctionBinding0<Object, ReturnType>(object, memberFunction));
        }

        template<typename Object, typename FunctionClass, typename P0, typename ReturnType>
        void bind(const std::string& name, Object* object, ReturnType (FunctionClass::*memberFunction)(P0))
        {
            setBinding(name, new LuaMemberFunctionBinding1<Object, P0, ReturnType>(object, memberFunction));
        }

        template<typename Object, typename FunctionClass, typename P0, typename P1, typename ReturnType>
        void bind(const std::string& name, Object* object, ReturnType (FunctionClass::*memberFunction)(P0, P1))
        {
            setBinding(name, new LuaMemberFunctionBinding2<Object, P0, P1, ReturnType>(object, memberFunction));
        }

        template<typename Object, typename FunctionClass, typename P0, typename P1, typename P2, typename ReturnType>
        void bind(const std::string& name, Object* object, ReturnType (FunctionClass::*memberFunction)(P0, P1, P2))
        {
            setBinding(name, new LuaMemberFunctionBinding3<Object, P0, P1, P2, ReturnType>(object, memberFunction));
        }

        template<typename Object, typename FunctionClass, typename P0, typename P1, typename P2, typename P3, typename ReturnType>
        void bind(const std::string& name, Object* object, ReturnType (FunctionClass::*memberFunction)(P0, P1, P2, P3))
        {
            setBinding(name, new LuaMemberFunctionBinding4<Object, P0, P1, P2, P3, ReturnType>(object, memberFunction));
        }

        template<typename Object, typename FunctionClass, typename P0, typename P1, typename P2, typename P3, typename P4, typename ReturnType>
        void bind(const std::string& name, Object* object, ReturnType (FunctionClass::*memberFunction)(P0, P1, P2, P3, P4))
        {
            setBinding(name, new LuaMemberFunctionBinding5<Object, P0, P1, P2, P3, P4, ReturnType>(object, memberFunction));
        }

        void unbind(const std::string& name);
        
        void call(const std::string& name);

    protected:
        typedef std::map<std::string, LuaFunctionBinding*> BindingMap;
        BindingMap mBindings;

        void setBinding(const std::string& name, LuaFunctionBinding* binding);

        static int luaEntryFunction(lua_State* L);
    };

} // namespace core

#endif // __LUAFUNCTIONCONTEXT_H__