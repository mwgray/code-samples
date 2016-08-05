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

    int Logger::severityColors[] =
    {
        0x000000, // kForce,
        0xAAAAAA, // kTrace,
        0x888888, // kDebug,
        0x000000, // kInfo,
        0x888800, // kWarn,
        0x880000, // kError,
        0xFF0000, // kFatal,
    };

    bool Logger::shouldLog(Severity severity, const CompactStringRelease& category)
    {
        bool isIgnored = ignoreCategories.IsSubscribedTo(category);
        bool isError = severity <= kError;

        return isError || (!isIgnored && (severity <= kWarn || IsSubscribedTo(category)));
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