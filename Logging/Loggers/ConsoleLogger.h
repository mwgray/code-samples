// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __CONSOLELOGGER_H__
#define __CONSOLELOGGER_H__

#include <PhyreDokiProfiling.h>

namespace core {

    class ConsoleLogger
        : public Logger
        , public Singleton<ConsoleLogger>
    {
    public:
        enum ErrorBehavior
        {
            kBreak,
            kIgnoreAll,
            kContinue,
        };

        ConsoleLogger();
        
        virtual ~ConsoleLogger();

        virtual void bindFunctions(LuaContext* context);

        ErrorBehavior logWithBreak(Severity severity, const char* category, const char* message, ...);

        virtual ErrorBehavior logVargs(Severity severity, const CompactStringDebug& category, const char* message, va_list& vargs);

        virtual ErrorBehavior handleSeverity(Severity severity, const CompactStringDebug& category, LargeStaticCharBuffer& buffer, int minDepth);
    };

} // namespace core

#ifdef __HNS_LOGGING_ENABLED__

#define logWrapper(severity, category, ...)                                                               \
    PHYRE_MULTI_LINE_MACRO_BEGIN                                                                          \
    {                                                                                                     \
        SCOPED_TRACKED_PHASE_EX(Logging);                                                                 \
        static core::LogMarker LINE_MARKER(logMarker);                                                    \
        if(LINE_MARKER(logMarker).logWithBreak(__FILE__, __LINE__, __FUNCTION__, severity, category, __VA_ARGS__)) \
        {                                                                                                 \
            PHYRE_BREAKPOINT;                                                                             \
        }                                                                                                 \
    }                                                                                                     \
    PHYRE_MULTI_LINE_MACRO_END

// used specifically for category "FPS", since this is reporting the cost of the other logging
#define  logInfoUntracked(category, message, ...) core::ConsoleLogger::Instance().info (category, message, ## __VA_ARGS__)

#else // __HNS_LOGGING_ENABLED__

#define logWrapper(severity, category, ...) DEAD_STRIP(severity); DEAD_STRIP(category); DEAD_STRIP_PARAMS(__VA_ARGS__);
#define logInfoUntracked(category, message, ...) DEAD_STRIP(category); DEAD_STRIP(message); DEAD_STRIP_PARAMS(__VA_ARGS__);

#endif // __HNS_LOGGING_ENABLED__

#define logFatal(category, ...) logWrapper(core::kFatal, category, __VA_ARGS__)
#define logError(category, ...) logWrapper(core::kError, category, __VA_ARGS__)
#define logForce(category, ...) logWrapper(core::kForce, category, __VA_ARGS__)
#define  logWarn(category, ...) logWrapper(core::kWarn,  category, __VA_ARGS__)
#define  logInfo(category, ...) logWrapper(core::kInfo,  category, __VA_ARGS__)
#define logDebug(category, ...) logWrapper(core::kDebug, category, __VA_ARGS__)
#define logTrace(category, ...) logWrapper(core::kTrace, category, __VA_ARGS__)

#endif // __CONSOLELOGGER_H__