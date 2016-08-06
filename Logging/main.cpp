#include "LoggingPch.h"

#include "SimpleLogger.h"

using namespace core;

void LogSession() {
    sinfo("Physics", "Aligning %d boson particles", 42);
    strace("Physics", "Aligning boson particles 0");
    strace("Physics", "Aligning boson particles 1");
    strace("Physics", "Aligning boson particles 3");
}

int main() {

    sforce("Example", "By default, the logger is not subscribed to any categories");
    sforce("Example", "But force logs are always shown");

    LogSession();

    sforce("Example", "Lets subscribe to physics");
    SimpleLogger::instance.SubscribeTo("Physics");

    LogSession();

    return 0;
}