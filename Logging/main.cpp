#include "LoggingPch.h"

#include "SimpleLogger.h"

using namespace core;

void LogSession(int count) {
    sinfo("Physics", "Aligning %d boson particles", count);
    for (int i = 0; i < count; ++i) {
        strace("Physics", "Aligning boson particles %d", i);
    }
}

int main() {

    sforce("Example", "By default, the logger is not subscribed to any categories");
    sforce("Example", "But force logs are always shown");

    LogSession(4);

    sforce("Example", "Lets subscribe to physics");
    SimpleLogger::instance.SubscribeTo("Physics");

    LogSession(4);

    return 0;
}