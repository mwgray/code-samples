// Copyright 2006-12 HumaNature Studios Inc.

#include "PhyreCorePch.h"

namespace core {

    Logger::Logger()
    {
    }

    Logger::~Logger()
    {
    }

    const char* Logger::priorityStrings[] =
    {
        "FATAL",
        "ERROR",
        "FORCE",
        "WARN",
        "INFO",
        "DEBUG",
        "TRACE",
    };

    int Logger::priorityColors[] =
    {
        0xFF0000, // kFatal,
        0x880000, // kError,    
        0x000000, // kForce,
        0x888800, // kWarn,
        0x000000, // kInfo,
        0x888888, // kDebug,
        0xAAAAAA, // kTrace,
    };

    bool Logger::shouldLog(Priority priority, const CompactStringRelease& category)
    {
        bool isIgnored = ignoreCategories.IsSubscribedTo(category);
        bool isError = priority <= kError;

        return isError || (!isIgnored && (priority <= kWarn || IsSubscribedTo(category)));
    }

    Logger::ErrorBehavior Logger::log(Priority priority, const char* category, const char* message, ...)
    {
        Logger::ErrorBehavior errorBehavior;
        
        va_list vargs; 
        va_start(vargs, message); 
        errorBehavior = logVargs(priority, category, message, vargs); 
        va_end(vargs);

        return errorBehavior;
    }

    void Logger::fatal(const char* category, const char* message, ...)
    {
        va_list vargs;
        va_start(vargs, message);
        logVargs(kFatal, category, message, vargs);
        va_end(vargs);
    }

    void Logger::error(const char* category, const char* message, ...)
    {
        va_list vargs;
        va_start(vargs, message);
        logVargs(kError, category, message, vargs);
        va_end(vargs);
    }

    void Logger::force(const char* category, const char* message, ...)
    {
        va_list vargs;
        va_start(vargs, message);
        logVargs(kForce, category, message, vargs);
        va_end(vargs);
    }

    void Logger::warn(const char* category, const char* message, ...)
    {
        va_list vargs;
        va_start(vargs, message);
        logVargs(kWarn , category, message, vargs);
        va_end(vargs);
    }

    void Logger::info(const char* category, const char* message, ...)
    {
        va_list vargs;
        va_start(vargs, message);
        logVargs(kInfo , category, message, vargs);
        va_end(vargs);
    }

    void Logger::debug(const char* category, const char* message, ...)
    {
        va_list vargs;
        va_start(vargs, message);
        logVargs(kDebug, category, message, vargs);
        va_end(vargs);
    }

    void Logger::trace(const char* category, const char* message, ...)
    {
        va_list vargs;
        va_start(vargs, message);
        logVargs(kTrace, category, message, vargs);
        va_end(vargs);
    }

    void Logger::ignore(const char* category)
    {
        ignoreCategories.SubscribeTo(category);
    }

} // namespace core