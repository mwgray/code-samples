#ifndef __SIMPLELOGGER_H__
#define __SIMPLELOGGER_H__

namespace core  {

    class SimpleLogger
        : public Logger
        , public Singleton<SimpleLogger>
    {
    public:
        virtual void logVargs(Severity severity, const CompactStringDebug& category, const char* message, va_list& vargs);
    };

} // namespace core

#ifdef __LOGGING_ENABLED__

#define strace(category, message, ...) SimpleLogger::Instance().log(Logger::kTrace, category, message, ## __VA_ARGS__)
#define sdebug(category, message, ...) SimpleLogger::Instance().log(Logger::kDebug, category, message, ## __VA_ARGS__)
#define sforce(category, message, ...) SimpleLogger::Instance().log(Logger::kForce, category, message, ## __VA_ARGS__)
#define  sinfo(category, message, ...) SimpleLogger::Instance().log(Logger::kInfo , category, message, ## __VA_ARGS__)
#define  swarn(category, message, ...) SimpleLogger::Instance().log(Logger::kWarn , category, message, ## __VA_ARGS__)
#define serror(category, message, ...) SimpleLogger::Instance().log(Logger::kError, category, message, ## __VA_ARGS__)
#define sfatal(category, message, ...) SimpleLogger::Instance().log(Logger::kFatal, category, message, ## __VA_ARGS__)

#else // __LOGGING_ENABLED__

#define strace(category, message, ...)
#define sdebug(category, message, ...)
#define sforce(category, message, ...)
#define  sinfo(category, message, ...)
#define  swarn(category, message, ...)
#define serror(category, message, ...)
#define sfatal(category, message, ...)

#endif // __LOGGING_ENABLED__

#endif // __SIMPLELOGGER_H__
