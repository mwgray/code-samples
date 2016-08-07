#include <cstdlib>
#include "LoggingPch.h"

#include "SimpleLogger.h"

using namespace core;
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

const char* lines[] = {
        "10",
        "-3",
        "37",
};

class Reader {

public:
    void ReadFile(const char* path, unsigned short*& output, unsigned int& outputCount) {
        sinfo("Reader", "Reading from file %s", path);
        outputCount = COUNT_OF(lines);

        sdebug("Reader", "File %s has %d lines", path, outputCount);
        strace("Reader", "Creating output array of length %d", outputCount);
        output = new unsigned short[outputCount];

        for(int i = 0; i < outputCount; ++i)
        {
            strace("Reader", "Reading line %d", i);
            sdebug("Reader", "Line %d is %s", i, lines[i]);
            auto asInt = atoi(lines[i]);
            strace("Reader", "Converted %s to %d", lines[i], asInt);

            output[i] = asInt;
            strace("Reader", "Stored %d in array as %u", asInt, output[i]);
        }

    }
};

class AverageAction {
public:
    unsigned short* inputs;
    unsigned int inputCount;

    unsigned int CalculateAverageFromFile(const char* path) {

        Reader reader;

        sinfo("Average", "Reading inputs from file");
        reader.ReadFile("/usr/tmp/file.txt", inputs, inputCount);
        sinfo("Average", "Read %d inputs", inputCount);

        return Execute();
    }

    unsigned int Execute() {
        sinfo("Average", "Averaging %u inputs", inputCount);
        int sum = 0;
        for (int i = 0; i < inputCount; ++i) {
            sdebug("Average", "Inputs %d = %u", i, inputs[i]);
            sum += inputs[i];
        }
        strace("Average", "Sum is %u", sum);

        int average = sum / inputCount;
        strace("Average", "Average is %u", sum);
        return average;
    }
};

void Test() {
    AverageAction averageAction;
    auto average = averageAction.CalculateAverageFromFile("/usr/tmp/file.txt");
    if(average > 100) {
        swarn("Average", "Average is unusually high!");
    }
    sinfo("Average", "Average is: %d -------------\n", average);
    printf("\n");
}

int main() {

    int testCount = 3;

    sforce("Example", "By default, the logger is not subscribed to any categories");
    sforce("Example", "But force logs are always shown");
    sforce("Example", "Lets subscribe to 'Example' so we can see logs without using force");

    SimpleLogger::instance.SubscribeTo("Example");

    sinfo("Example", "Ah that's better!  Lets run a quick averaging test");

    Test();

    sinfo("Example", "Hmm, looks like there is something wrong with the average.");
    sinfo("Example", "By default warnings and above are shown regardless of subscriton.");
    sinfo("Example", "But there is no output for the averager, so lets subscribe to that channel");

    SimpleLogger::instance.SubscribeTo("Average");

    Test();

    sinfo("Example", "Hmm, not much there, so lets increase the verbosity of the log");

    SimpleLogger::instance.hideBelow = Logger::kDebug;

    Test();

    sinfo("Example", "Ah looks like the input is wrong.");
    sinfo("Example", "At this point its not clear what is causing the error, so lets subscribe to all channels.");
    SimpleLogger::instance.SubscribeToAll();

    Test();

    sinfo("Example", "Looks like there's an issue with the reader.");
    sinfo("Example", "Seems like the reader has a unsigned/signed issue.");
    sinfo("Example", "Lets increase the verbosity of the log once more.");
    sinfo("Example", "In addition, since we know that the issue isn't with the Averager,");
    sinfo("Example", "We don't need to see it's logs anymore.  We can unsubscribe from the channel");
    sinfo("Example", "but since we're subscribe to all channels it will still output.");
    sinfo("Example", "Instead, let's ignore the channel, which will also mute the initial warning.");

    SimpleLogger::instance.hideBelow = Logger::kTrace;
    SimpleLogger::instance.ignore("Average");

    Test();

    sinfo("Example", "Bingo!  Now we know that there's a signing issue, it's storing -3 as 65533. Bug found!\n");

    sinfo("Example", "This example isn't exactly as you would see it in the real world, but an illustrative example. You would likely\n"
            "have a Logger that outputs to the TTY for what's important to the local programmer, and a Logger\n"
            "that saves to a file with all log statements, and a Logger that displays frequenly changed values to the\n"
            "screen");

    return 0;
}