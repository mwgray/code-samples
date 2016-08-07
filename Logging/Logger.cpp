// Copyright 2006-12 HumaNature Studios Inc.

#include "LoggingPch.h"

namespace core {

    Logger::Logger()
        : showAbove(kInfo)
        , hideBelow(kInfo)
    {
    }

    Logger::~Logger()
    {
    }

    const char* Logger::severityStrings[] =
    {
        "FORCE",
        "TRACE",
        "DEBUG",
        "INFO",
        "WARN",
        "ERROR",
        "FATAL",
    };

    void Logger::ignore(const char* category)
    {
        ignoreCategories.SubscribeTo(category);
    }

    bool Logger::shouldLog(Severity severity, const CompactStringRelease& category)
    {
        if(severity == kForce)
        {
            // force is always shown
            return true;
        }

        if(ignoreCategories.IsSubscribedTo(category))
        {
            // is an ignored category
            return false;
        }

        if(severity > showAbove)
        {
            return true;
        }

        if(severity < hideBelow)
        {
            return false;
        }

        return IsSubscribedTo(category);
    }

    void Logger::log(Severity severity, const char* category, const char* message, ...)
    {
        if(shouldLog(severity, category))
        {
            va_list vargs;
            va_start(vargs, message);
            logVargs(severity, category, message, vargs);
            va_end(vargs);
        }
    }

} // namespace core