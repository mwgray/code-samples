// Copyright 2006-13 HumaNature Studios Inc.

#include "CorePch.h"

#include "LuaTestLib.h"

#include "LuaTestHarness.h"

namespace core {

    REGISTER_FACTORY_CLASS_NS(core, LuaTestLib, Castable);

    LuaTestLib::LuaTestLib() 
        : mLuaBinding(nullptr)
    {

    }

    LuaTestLib::~LuaTestLib()
    {

    }

    void LuaTestLib::sendEvent(const char* format, ...)
    {
        // TODO - ugh, not the best here...  cleanup later
        LargeStaticCharBuffer largeEventStringBuffer;
        char * eventString = largeEventStringBuffer.getBuffer();
        va_list vargs; 

        va_start(vargs, format); 
        PHYRE_VSNPRINTF(eventString, LargeStaticCharBuffer::kBufferSize, format, vargs);
        va_end(vargs);

        LargeStaticCharBuffer largeLuaCodeBuffer;
        char * luaCode = largeLuaCodeBuffer.getBuffer();
        PHYRE_SNPRINTF(luaCode, LargeStaticCharBuffer::kBufferSize, "harness.sendEvent('%s');", eventString);

        mLuaBinding->executeString(luaCode);
    }

    void LuaTestLib::bind(LuaContext* luaBinding)
    {
        assertThat(luaBinding, isNotNull);

        mLuaBinding = luaBinding;
    }

    void LuaTestLib::log(const char* message, ...)
    {
        LargeStaticCharBuffer largeEventStringBuffer;
        va_list vargs; 

        va_start(vargs, message); 
        PHYRE_VSNPRINTF(largeEventStringBuffer.getCumulativeBuffer(), largeEventStringBuffer.getRemainingBuffer(), message, vargs);
        
        // send to logger
        ConsoleLogger::Instance().logVargs(kInfo, "Test", message, vargs);
        va_end(vargs);

        // send to AS3
        LuaTestHarness::Instance().logBackToAS3(largeEventStringBuffer.getBuffer());        
    }

} // namespace core