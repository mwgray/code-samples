// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __LOGGER_H__
#define __LOGGER_H__

namespace core  {

    //TODO: check the order of all of these, and any <= operations
    enum Priority
    {
        kForce, // force is a special case, it will always be logged

        kTrace,
        kDebug,
        kInfo,
        kWarn,
        kError,
        kFatal,
    };

    class Logger
        : public Subscription
    {
    public:
        enum ErrorBehavior
        {
            kBreak,
            kIgnoreAll,
            kContinue,
        };

        Logger();
        
        virtual ~Logger();

        virtual ErrorBehavior logVargs(Priority priority, const CompactStringDebug& category, const char* message, va_list& vargs) = 0;

        bool shouldLog(Priority priority, const CompactStringRelease& category);

        ErrorBehavior log(Priority priority, const char* category, const char* message, ...);

        void fatal(const char* category, const char* message, ...);
        void error(const char* category, const char* message, ...);
        void force(const char* category, const char* message, ...);
        void warn (const char* category, const char* message, ...);
        void info (const char* category, const char* message, ...);
        void debug(const char* category, const char* message, ...);
        void trace(const char* category, const char* message, ...);

        void ignore(const char* category);

    protected:
        static const char* priorityStrings[];

        // TODO - color
        static int priorityColors[];

        Subscription ignoreCategories;
    };

} // namespace core

#endif // __LOGGER_H__