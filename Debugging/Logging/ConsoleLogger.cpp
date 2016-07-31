// Copyright 2006-12 HumaNature Studios Inc.

#include "PhyreCorePch.h"
#include "HumaNature/HumaNature.h"

#include "HumaNature/OS/PlatformTimer.h"
#include "HumaNature/Debug/StackTracer.h"
#include "HumaNature/Logging/PlatformPrint.h"

DEFINE_SINGLETON_TYPE(core, ConsoleLogger);

namespace core {

    ConsoleLogger::ConsoleLogger()
    {
        PlatformPrint::initialize();
    }

    ConsoleLogger::~ConsoleLogger()
    {
        PlatformPrint::finalize();
    }

    void ConsoleLogger::bindFunctions(LuaFunctionBinder* binder)
    {
        binder->bind("log", this, &ConsoleLogger::SubscribeTo);
        binder->bind("logAll", this, &ConsoleLogger::SubscribeToAll);
        binder->bind("ignore", this, &ConsoleLogger::ignore);
    }

    Logger::ErrorBehavior ConsoleLogger::logVargs(Priority priority, const CompactStringDebug& category, const char* message, va_list& vargs)
    {
        if(shouldLog(priority, category))
        {
            LargeStaticCharBuffer buffer;

            buffer.clear();

            // append the header
            PHYRE_SNPRINTF(buffer.getCumulativeBuffer(), buffer.getRemainingBuffer(), "%d\t%s\t%s\t", PlatformTimer::NowMilliseconds(), priorityStrings[priority], category.c_str());
            
            // append and format the message
            PHYRE_VSNPRINTF(buffer.getCumulativeBuffer(), buffer.getRemainingBuffer(), message, vargs);

            // append a newline
            PHYRE_STRNCAT(buffer.getCumulativeBuffer(), "\n", buffer.getRemainingBuffer());

            // send the message to the TTY
            PlatformPrint::print(buffer.getBuffer(), priority);

            return handlePriority(priority, category, buffer, 5);
        }
        else
        {
            return kContinue;
        }
    }

    Logger::ErrorBehavior ConsoleLogger::handlePriority(Priority priority, const CompactStringDebug& category, LargeStaticCharBuffer& buffer, int minDepth)
    {
        UNUSED_PARAM(category);
        UNUSED_PARAM(buffer);

        if(priority <= kError)
        {
            StackTracer::Instance().PlatformPrintStack(priority, nullptr, minDepth);
        }

        if(priority <= kFatal)
        {
            if(!BuildPlatform::IsDebuggerPresent())
            {
                PlatformPrint::print("Exiting due to fatal.\n", kFatal);
                BuildPlatform::Exit(1);
            }
        }

        return priority <= kError ? kBreak : kContinue;
    }

} // namespace core