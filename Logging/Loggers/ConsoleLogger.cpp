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

    void ConsoleLogger::bindFunctions(LuaContext* context)
    {
        context->bind("log", this, &ConsoleLogger::SubscribeTo);
        context->bind("logAll", this, &ConsoleLogger::SubscribeToAll);
        context->bind("ignore", this, &ConsoleLogger::ignore);
    }

    Logger::ErrorBehavior Logger::logWithBreak(Severity severity, const char* category, const char* message, ...)
    {
        Logger::ErrorBehavior errorBehavior;

        va_list vargs;
        va_start(vargs, message);
        errorBehavior = logVargs(severity, category, message, vargs);
        va_end(vargs);

        return errorBehavior;
    }

    Logger::ErrorBehavior ConsoleLogger::logVargs(Severity severity, const CompactStringDebug& category, const char* message, va_list& vargs)
    {
        if(shouldLog(severity, category))
        {
            LargeStaticCharBuffer buffer;

            buffer.clear();

            // append the header
            PHYRE_SNPRINTF(buffer.getCumulativeBuffer(), buffer.getRemainingBuffer(), "%d\t%s\t%s\t", PlatformTimer::NowMilliseconds(), severityStrings[severity], category.c_str());
            
            // append and format the message
            PHYRE_VSNPRINTF(buffer.getCumulativeBuffer(), buffer.getRemainingBuffer(), message, vargs);

            // append a newline
            PHYRE_STRNCAT(buffer.getCumulativeBuffer(), "\n", buffer.getRemainingBuffer());

            // send the message to the TTY
            PlatformPrint::print(buffer.getBuffer(), severity);

            return handleSeverity(severity, category, buffer, 5);
        }
        else
        {
            return kContinue;
        }
    }

    Logger::ErrorBehavior ConsoleLogger::handleSeverity(Severity severity, const CompactStringDebug& category, LargeStaticCharBuffer& buffer, int minDepth)
    {
        UNUSED_PARAM(category);
        UNUSED_PARAM(buffer);

        if(severity <= kError)
        {
            StackTracer::Instance().PlatformPrintStack(severity, nullptr, minDepth);
        }

        if(severity <= kFatal)
        {
            if(!BuildPlatform::IsDebuggerPresent())
            {
                PlatformPrint::print("Exiting due to fatal.\n", kFatal);
                BuildPlatform::Exit(1);
            }
        }

        return severity <= kError ? kBreak : kContinue;
    }

} // namespace core