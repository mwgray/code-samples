// Copyright 2006-13 HumaNature Studios Inc.

#ifndef __LUATESTLIB_H__
#define __LUATESTLIB_H__

#include "Scripting/LuaContext.h"

namespace core {

    class LuaTestLib
        : public MemoryObject
        , public Factory<LuaTestLib, CompactString, LuaTestLib*(*)()>
    {
    public:
        LuaTestLib();

        virtual ~LuaTestLib();

        void sendEvent(const char* format, ...);

        virtual void update(float deltaSeconds) { UNUSED_PARAM(deltaSeconds); }

    protected:
        LuaContext* mLuaBinding;

        virtual void bind(LuaContext* luaBinding);

        // for logging back to to cheats & console
        void log(const char* message, ...);

        friend class LuaTestHarness; // for LuaTestHarness::createScript(const char*);
    };

} // namespace core

#endif // __LUATESTLIB_H__