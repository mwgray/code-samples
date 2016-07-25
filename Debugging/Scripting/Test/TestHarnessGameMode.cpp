// Copyright 2006-13 HumaNature Studios Inc.

#include "CorePch.h"

#include "GameMode/GameMode.h"

#include "LuaTestHarness.h"
#include "Scripting/LuaScriptManager.h"
#include "Phyre/PhyreFileLoader.h"

namespace core {

    class TestHarnessGameMode
        : public GameMode
    {
    public:
        StringVec mBootParams;

        virtual void Initialize(PropertyObject* properties)
        {
            GameMode::Initialize(properties);

            PhyreFileLoader::Instance().addEventListener(PhyreFileLoader::ALL_LOADS_COMPLETE, this, &TestHarnessGameMode::onLoadsComplete, true);
        }

        void onLoadsComplete(const Event*)
        {	
            CoreLuaScript* globalContext = LuaScriptManager::Instance().getGlobalContext();
            globalContext->getProperties()->readVec("bootParams", mBootParams);

            std::string testScriptPath = mBootParams[0];
            testScriptPath = "common/scripts/test/" + testScriptPath + ".lua";

            mBootParams.erase(mBootParams.begin());

            LuaTestHarness::Instance().runTestScript(testScriptPath.c_str(), mBootParams);
        }

        virtual void Finalize()
        {
            GameMode::Finalize();
        }
    };

    REGISTER_FACTORY_CLASS_NS(core, TestHarnessGameMode, GameMode);

} // namespace core
