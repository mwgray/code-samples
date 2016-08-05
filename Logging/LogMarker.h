// Copyright 2006-13 HumaNature Studios Inc.

#ifndef __LOGMARKER_H__
#define __LOGMARKER_H__

namespace core {

    class LogMarker
    {
    public:
        bool enabled;

        LogMarker();

        bool log(const char* file, int line, const char* function, Severity severity, const char* category, const char* message, ...);
    };

} // namespace core

#endif // __LOGMARKER_H__