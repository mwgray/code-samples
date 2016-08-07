#include <cstdlib>
#include "LoggingPch.h"

#include "SimpleLogger.h"

using namespace core;

void InputTest(int count = 4) {
    sinfo("UI", "User is checking inputs");
    sinfo("Input", "Checking %d inputs", count);
    swarn("Input", "There is a bad input count");
    sinfo("UI", "Asking user if it is safe to continue");
    sinfo("UI", "User said to continue");
    sinfo("Input", "UI ignored safeguards");

    for (int i = 0; i < count; ++i) {
        strace("Input", "input %d is %s", i, i == count - 1 ? "bad!" : "good.");
        strace("UI", "Showing input %d", i);
    }
    serror("Input", "Error reading inputs!");
    sfatal("UI", "UI crashed due to bad input");

    sforce("Example", "Done.\n");
}

const char* lines[] = {
    "10",
    "-3",
    "37",
};

#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

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
            strace("Reader", "Line %d is %s", i, lines[i]);
            auto asInt = atoi(lines[i]);
            strace("Reader", "Converted %s to %d", lines[i], asInt);

            output[i] = asInt;
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
            strace("Average", "Inputs %d = %u", i, inputs[i]);
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

    SimpleLogger::instance.hideBelow = Logger::kTrace;

    Test();

//    SimpleLogger::instance.SubscribeToAll();
//    SimpleLogger::instance.hideBelow = Logger::kTrace;

    Test();return 0;

    sforce("Example", "Looks like something went wrong with input, Lets subscribe to the channel Input");
    SimpleLogger::instance.SubscribeTo("Input");

    InputTest(testCount);

    SimpleLogger::instance.hideBelow = Logger::kTrace;
    InputTest(testCount);

    return 0;
}