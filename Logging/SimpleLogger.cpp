// Copyright 2006-12 HumaNature Studios Inc.

#include "LoggingPch.h"

#include "SimpleLogger.h"

namespace core {

    DEFINE_SINGLETON_TYPE(core, SimpleLogger);

    void SimpleLogger::logVargs(Severity severity, const CompactStringDebug &category, const char *message, va_list &vargs)
    {
        // print severity / category
        printf("%s\t[%s]\t", severityStrings[severity], category.c_str());

        // print the formatted message
        vprintf(message, vargs);

        // append a new-line
        printf("\n");
    };

} // namespace core

