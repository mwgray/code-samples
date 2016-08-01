// Copyright 2006-13 HumaNature Studios Inc.

#include "PhyreCorePch.h"

#include "HumaNature/Logging/PlatformPrint.h"

namespace core {

#ifdef __HNS_WIN32__

    static HANDLE gOutputHandle = INVALID_HANDLE_VALUE;
    static const WORD gDefaultAttib = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    static WORD gPriorityCharacterAttribs[] =
    {
        FOREGROUND_INTENSITY | FOREGROUND_RED,// kFatal,
        FOREGROUND_INTENSITY | FOREGROUND_RED,// kError,
        gDefaultAttib, // kForce,
        FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,// kWarn,
        gDefaultAttib, // kInfo,
        gDefaultAttib, // kDebug,
        gDefaultAttib, // kTrace,
    };
    static bool gOutputFileCreated = false;
    static const char gOutputFilePath[] = "C:/temp/dokilog.txt";
    static const char gOutputFileTestPath[] = "C:/temp/iwantdokilogging.txt";
    static CRITICAL_SECTION gCriticalSection;

#endif // __HNS_WIN32__

    void PlatformPrint::initialize()
    {
#ifdef __HNS_WIN32__
        InitializeCriticalSection(&gCriticalSection);
        if(AttachConsole(ATTACH_PARENT_PROCESS))
        {
            gOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        }

        FILE* testFile = NULL;
        if (0 == fopen_s(&testFile,gOutputFileTestPath,"r"))
        {
            fclose(testFile);

            FILE* outputFile = NULL;
            if (0 == fopen_s(&outputFile, gOutputFilePath,"wt"))
            {
                fclose(outputFile);
                gOutputFileCreated = true;
            }
        }

#if defined __HNS_USESTACKTRACE__
        StackTracer::Instance().initialize();
#endif // __HNS_USESTACKTRACE__

#endif
    }


    void PlatformPrint::finalize()
    {
#ifdef __HNS_WIN32__
        if (gOutputFileCreated)
        {
            // create string file
            const char gStringTableFilePath[] = "C:/temp/dokistrings.txt";
            CompactStringHolderWriteToFile(gStringTableFilePath);
        }
        DeleteCriticalSection(&gCriticalSection);
#endif //__HNS_WIN32__
    }

    void PlatformPrint::print(const char* message, int priority)
    {
#ifdef __HNS_WIN32__
        EnterCriticalSection(&gCriticalSection);
        OutputDebugStringA(message);
        std::fputs(message, stdout);
        fflush(stdout);
        if(gOutputHandle != INVALID_HANDLE_VALUE)
        {
            SetConsoleTextAttribute(gOutputHandle, gPriorityCharacterAttribs[priority]);
            WriteFile(gOutputHandle, message, (DWORD)strlen(message), nullptr, nullptr);
            SetConsoleTextAttribute(gOutputHandle, gDefaultAttib);
        }
        if (gOutputFileCreated)
        {
            FILE* outputFile = NULL;
            if (0 == fopen_s(&outputFile, gOutputFilePath,"at"))
            {
                fputs(message, outputFile);
                fclose(outputFile);
            }
        }
        LeaveCriticalSection(&gCriticalSection);
#else //! __HNS_WIN32__
        (void)priority;
        std::fputs(message, stdout);
#endif //! __HNS_WIN32__
    }

} // namespace core