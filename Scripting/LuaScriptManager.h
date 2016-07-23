// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __LUASCRIPTMANAGER_H__
#define __LUASCRIPTMANAGER_H__

#include "LuaFunctionBinder.h"

namespace core {

    class LuaFunctionBinder;

    class LuaScriptManager
        : public MemoryObject
        , public Singleton<LuaScriptManager>
    {
    public:
        void initialize();

        void update(float elapsedTime);

        void finalize();

        void runScriptString(const char* commandBuffer);

        inline LuaFunctionBinder* getGlobalContext()
        {
            return getContext("global");
        }

        LuaFunctionBinder* getContext(const char* context);
        
        bool freeContext(const char* context);

    protected:
        typedef std::map<std::string, LuaFunctionBinder*> BinderMap;
        BinderMap mBinderMap;

        void bindToDebug();
    };

} // namespace core

#endif // __LUASCRIPTMANAGER_H__