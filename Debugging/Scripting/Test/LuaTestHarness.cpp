// Copyright 2006-13 HumaNature Studios Inc.

#include "CorePch.h"

#include "LuaTestHarness.h"
#include "Logging/GameLogger.h"
#include "Scaleform/ScaleformValueWrapper.h"

DEFINE_SINGLETON_TYPE(core, LuaTestHarness);

namespace core {

    LuaTestHarness::LuaTestHarness() 
        : mLuaBinding(nullptr)
    {
        mCheatUIHarness.SetNull();
    }

    LuaTestHarness::~LuaTestHarness()
    {
        mCheatUIHarness.SetNull();
    }

    void LuaTestHarness::finalize()
    {
        // bind the functions
        for(LuaTestLibList::iterator i = mLuaTestLibList.begin(); i != mLuaTestLibList.end(); ++i)
        {
            LuaTestLib* testLib = *i;
            SafeDelete(testLib);
        }

        SafeDelete(mLuaBinding);
    }

    void LuaTestHarness::initializeCheats(Scaleform::GFx::Value cheatUIHarness)
    {
        mCheatUIHarness = cheatUIHarness;

        if(mLuaBinding == nullptr)
        {
            createScript("common/scripts/cheats/CheatHarness.lua");
            mLuaBinding->bind("addCheat", this, &LuaTestHarness::addCheat);
        }
        mLuaBinding->clearProperties();
        mLuaBinding->executeFile("common/scripts/cheats/Cheats.lua");

        ScaleformValueUtil::Invoke(mCheatUIHarness, "cheatsLoaded");
    }

    void LuaTestHarness::finalizeCheats()
    {
        mCheatUIHarness.SetNull();
    }

    void LuaTestHarness::runCheatScript(const char* script)
    {
        PStringBuilder luaCode;
        luaCode = "harness.";
        luaCode += script;

        mLuaBinding->executeString(luaCode.c_str());
    }

    void LuaTestHarness::runTestScript(const char* path, const StringVec& params)
    {
        assertThat(mLuaBinding, isNull, "Running more that one test script at once!");
        createScript(path);

        PStringBuilder luaCode;
        luaCode = "harness.executeTest(";

        for(StringVec::const_iterator i = params.begin(); i != params.end(); ++i)
        {
            if(i != params.begin())
            {
                luaCode += ", ";
            }

            const std::string& param = *i;
            luaCode += param.c_str();
        }

        luaCode += ")";

        mLuaBinding->executeString(luaCode.c_str());
    }

    void LuaTestHarness::update(float deltaSeconds)
    {
        for(LuaTestLibList::iterator i = mLuaTestLibList.begin(); i != mLuaTestLibList.end(); ++i)
        {
            LuaTestLib* testLib = *i;
            testLib->update(deltaSeconds);
        }
    }

    void LuaTestHarness::createScript(const char* path)
    {
        mLuaBinding = HNS_NEW(LuaContext);
#ifdef __HNS_LOGGING_ENABLED__
        ConsoleLogger::Instance().bindFunctions(mLuaBinding);
#endif

        mLuaBinding->executeFile("common/scripts/test/TestHarness.lua");
        
        // TODO - need to separate lua from the prop creator
        mLuaBinding->clearProperties();

        mLuaBinding->popToTop();
        mLuaBinding->bind("import", this, &LuaTestHarness::importLib);
        mLuaBinding->executeFile(path);
        mLuaBinding->popToTop();

        // bind the functions
        for(LuaTestLibList::iterator i = mLuaTestLibList.begin(); i != mLuaTestLibList.end(); ++i)
        {
            LuaTestLib* testLib = *i;
            testLib->bind(mLuaBinding);
        }
    }

    void LuaTestHarness::importLib(const char* libName)
    {
        LuaTestLib* luaTestLib = LuaTestLib::Create(libName);
        mLuaTestLibList.push_back(luaTestLib);
    }

    void LuaTestHarness::addCheat(const char* category, const char* label, const char* luaCode)
    {
        if(!mCheatUIHarness.IsNull())
        {
            ScaleformValueUtil::Invoke(mCheatUIHarness, "addCheat", category, label, luaCode);
        }   
    }

    void LuaTestHarness::logBackToAS3(const char* message)
    {
        if(!mCheatUIHarness.IsNull())
        {
            ScaleformValueUtil::Invoke(mCheatUIHarness, "logFromCpp", message);
        }
    }

} // namespace core