// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __LOGGER_H__
#define __LOGGER_H__

namespace core  {

    class Logger
        : public Subscription
    {
    public:
        //TODO: check the order of all of these, and any <= operations
        enum Severity
        {
            kForce, // force is a special case, it will always be logged

            kTrace,
            kDebug,
            kInfo,
            kWarn,
            kError,
            kFatal,
        };

        enum ErrorBehavior
        {
            kBreak,
            kIgnoreAll,
            kContinue,
        };

        Logger();
        
        virtual ~Logger();

        virtual ErrorBehavior logVargs(Severity severity, const CompactStringDebug& category, const char* message, va_list& vargs) = 0;

        bool shouldLog(Severity severity, const CompactStringRelease& category);

        ErrorBehavior log(Severity severity, const char* category, const char* message, ...);

        void force(const char* category, const char* message, ...);
        void trace(const char* category, const char* message, ...);
        void debug(const char* category, const char* message, ...);
        void info (const char* category, const char* message, ...);
        void warn (const char* category, const char* message, ...);
        void error(const char* category, const char* message, ...);
        void fatal(const char* category, const char* message, ...);

        void ignore(const char* category);

    protected:
        static const char* severityStrings[];
        static int severityColors[];

        Subscription ignoreCategories;
    };

} // namespace core

#endif // __LOGGER_H__