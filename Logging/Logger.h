// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __LOGGER_H__
#define __LOGGER_H__

namespace core  {

    class Logger
        : public Subscription
    {
    public:
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

        Logger();
        
        virtual ~Logger();

        void ignore(const char* category);

        bool shouldLog(Severity severity, const CompactStringRelease& category);

        void log(Severity severity, const char* category, const char* message, ...);

        virtual void logVargs(Severity severity, const CompactStringDebug& category, const char* message, va_list& vargs) = 0;

    protected:
        static const char* severityStrings[];

        Subscription ignoreCategories;
    };

} // namespace core

#endif // __LOGGER_H__