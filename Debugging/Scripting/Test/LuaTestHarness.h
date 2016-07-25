// Copyright 2006-13 HumaNature Studios Inc.

#ifndef __LUATESTHARNESS_H__
#define __LUATESTHARNESS_H__

#include "LuaTestLib.h"

namespace core {

    class LuaTestHarness :
        public Singleton<LuaTestHarness>
    {
    public:
        LuaTestHarness();

        ~LuaTestHarness();

        void finalize();

        void initializeCheats(Scaleform::GFx::Value cheatUIHarness);

        void finalizeCheats();

        void runCheatScript(const char* script);

        void runTestScript(const char* path, const StringVec& params);

        void update(float deltaSeconds);

    protected:
        typedef std::list<LuaTestLib*> LuaTestLibList;
        LuaTestLibList mLuaTestLibList;
        Scaleform::GFx::Value mCheatUIHarness;

        LuaFunctionBinder* mLuaBinding;

        void createScript(const char* path);

        void importLib(const char* libName);

        void addCheat(const char* category, const char* label, const char* luaCode);

        friend class LuaTestLib;
        void logBackToAS3(const char* message);
    };

} // namespace core

#endif // __LUATESTHARNESS_H__