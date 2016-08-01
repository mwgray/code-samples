// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __LUASCRIPTMANAGER_H__
#define __LUASCRIPTMANAGER_H__

#include "LuaContext.h"

namespace core {

    class LuaContext;

    class LuaScriptManager
        : public MemoryObject
        , public Singleton<LuaScriptManager>
    {
    public:
        void initialize();

        void update(float elapsedTime);

        void finalize();

        void runScriptString(const char* commandBuffer);

        inline LuaContext* getGlobalContext()
        {
            return getContext("global");
        }

        LuaContext* getContext(const char* context);
        
        bool freeContext(const char* context);

    protected:
        typedef std::map<std::string, LuaContext*> ContextMap;
        ContextMap mContextMap;

        void bindToDebug();
    };

} // namespace core

#endif // __LUASCRIPTMANAGER_H__