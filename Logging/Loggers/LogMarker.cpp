// Copyright 2006-13 HumaNature Studios Inc.

#include "PhyreCorePch.h"

namespace core {

    LogMarker::LogMarker() 
        : enabled(true)
    {

    }

    bool LogMarker::log(const char* file, int line, const char* function, Severity severity, const char* category, const char* message, ...)
    {
        if(message && enabled && ConsoleLogger::Instance().shouldLog(severity, category))
        {
            LargeStaticCharBuffer buffer;
            buffer.clear();

            va_list vargs; 
            va_start(vargs, message); 
            {
                // append the message
                PHYRE_VSNPRINTF(buffer.getCumulativeBuffer(), buffer.getRemainingBuffer(), message, vargs);
            }
            va_end(vargs);

            if(severity <= kWarn
                && severity != kForce)
            {
                // show MSVC click link
                PHYRE_SNPRINTF(buffer.getCumulativeBuffer(), buffer.getRemainingBuffer(), "\n  %s(%d): %s", file, line, function);
            }

            Logger::ErrorBehavior errorBehavior = ConsoleLogger::Instance().log(severity, category, "%s", buffer.getBuffer());

            switch(errorBehavior)
            {
            case ConsoleLogger::kBreak:
                return true;

            case ConsoleLogger::kIgnoreAll:
                enabled = false;
                break;

            case ConsoleLogger::kContinue:
                break;
            }
        }

        return false;
    }

} // namespace core