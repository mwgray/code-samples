// Copyright 2006-12 HumaNature Studios Inc.

#include "CorePch.h"

#include "LuaScriptManager.h"

DEFINE_SINGLETON_TYPE(core, LuaScriptManager);

namespace core {
    void LuaScriptManager::initialize()
    {
        bindToDebug();
    }

    void LuaScriptManager::update(float)
    {
        static bool tryConsoleFile = true;
        if (!tryConsoleFile)
        {
            return;
        }

        PStreamReaderFile consoleFile("console");

        if(consoleFile.isOpen())
        {
            static PInt64 modTime = consoleFile.getModTime();
            static int caretPos = consoleFile.getRemainingSize();
            int fileSize = consoleFile.getRemainingSize();

            if(modTime != consoleFile.getModTime()
                || fileSize != caretPos)
            {
                if(consoleFile.getRemainingSize() == 0)
                {
                    caretPos = 0;
                }

                modTime = consoleFile.getModTime();
                // file updated, execute
                // first read, skip to end
                consoleFile.PStreamFile::skip(caretPos);

                char commandBuffer[255];

                memset(commandBuffer, 0, sizeof(commandBuffer));
                int readBytes = consoleFile.read(commandBuffer, 256);
                if(readBytes > 0)
                {
                    caretPos += readBytes;
                    logInfo("Lua CLI", "Read %s from CLI", commandBuffer);

                    runScriptString(commandBuffer);
                }
            }

            consoleFile.close();
        }
        else
        {
            tryConsoleFile = false;
        }
    }

    void LuaScriptManager::finalize()
    {
        for (auto iter = mContextMap.begin(); iter != mContextMap.end(); ++iter)
        {
            logInfo("Lua", "Bound context: '%s'", iter->first.c_str());

            SafeDelete(iter->second);
        }
    }

    void LuaScriptManager::runScriptString(const char* string)
    {
        char commandBuffer[256];
        PHYRE_STRNCPY(commandBuffer, string, sizeof(commandBuffer));

        char* context = commandBuffer;
        char* command = strstr(context, "::");
        if(command == nullptr)
        {
            context = nullptr;
            command = commandBuffer;
        }
        else
        {
            // bump to the command
            command[0] = 0;
            command += 2;
        }

        if(context != nullptr)
        {
            LuaScriptManager::Instance().getContext(context)->executeString(command);
        }
        else
        {
            LuaScriptManager::Instance().getGlobalContext()->executeString(command);
        }
    }

    LuaContext* LuaScriptManager::getContext(const char* context)
    {
        LuaContext* context = nullptr;
        ContextMap::iterator find = mContextMap.find(context);

        if(find == mContextMap.end())
        {
            context = HNS_NEW(LuaContext)();
            mContextMap[context] = context;
        }
        else
        {
            context = find->second;
        }

        return context;
    }

    void LuaScriptManager::bindToDebug()
    {
#ifdef __HNS_LOGGING_ENABLED__
        LuaContext* context = getGlobalContext();

        ConsoleLogger::Instance().bindFunctions(context);
        ScreenReporter::Instance().bindFunctions(context);
        ScreenText::Instance().bindFunctions(context);
        DebugDraw::Instance().bindFunctions(context);
#endif
    }

    bool LuaScriptManager::freeContext(const char* context)
    {
        ContextMap::iterator find = mContextMap.find(context);

        if(find != mContextMap.end())
        {
            SafeDelete(find->second);
            mContextMap.erase(find);

            return true;
        }
        else
        {
            return false;
        }
    }
} // namespace core