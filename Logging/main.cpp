#include "LoggingPch.h"

using namespace core;

class ExampleLogger
        : public Logger {

    // log the message.  returns how the program should proceed if the log is a error or above
    virtual ErrorBehavior logVargs(Priority priority, const CompactStringDebug& category, const char* message, va_list& vargs)
    {
        // print priority / category
        printf("%s\t[%s]\t", priorityStrings[priority], category.c_str());

        // print the formatted message
        vprintf(message, vargs);

        // append a new-line
        printf("\n");

        return kContinue;
    }
};

ExampleLogger logger;

void LogSession() {
    logger.info("Physics", "Aligning %d boson particles", 42);
    logger.trace("Physics", "Aligning boson particles 0");
    logger.trace("Physics", "Aligning boson particles 1");
    logger.trace("Physics", "Aligning boson particles 3");
}

int main() {

    logger.force("Example", "By default, the logger is not subscribed to any categories");
    logger.force("Example", "But force logs are always shown");

    LogSession();

    logger.force("Example", "Lets subscribe to physics");
    logger.SubscribeTo("Physics");

    LogSession();

    return 0;
}