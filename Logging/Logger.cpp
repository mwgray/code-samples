// Copyright 2006-12 HumaNature Studios Inc.

#include "LoggingPch.h"

namespace core {

    Logger::Logger()
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
        bool isIgnored = ignoreCategories.IsSubscribedTo(category);
        bool isError = severity <= kError;

        return isError || (!isIgnored && (severity <= kWarn || IsSubscribedTo(category)));
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