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
        for (auto iter = mBinderMap.begin(); iter != mBinderMap.end(); ++iter)
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

    LuaFunctionBinder* LuaScriptManager::getContext(const char* context)
    {
        LuaFunctionBinder* binder = nullptr;
        BinderMap::iterator find = mBinderMap.find(context);

        if(find == mBinderMap.end())
        {
            binder = HNS_NEW(LuaFunctionBinder)();
            mBinderMap[context] = binder;
        }
        else
        {
            binder = find->second;
        }

        return binder;
    }

    void LuaScriptManager::bindToDebug()
    {
#ifdef __HNS_LOGGING_ENABLED__
        LuaFunctionBinder* binder = getGlobalContext();

        ConsoleLogger::Instance().bindFunctions(binder);
        ScreenReporter::Instance().bindFunctions(binder);
        ScreenText::Instance().bindFunctions(binder);
        DebugDraw::Instance().bindFunctions(binder);
#endif
    }

    bool LuaScriptManager::freeContext(const char* context)
    {
        BinderMap::iterator find = mBinderMap.find(context);

        if(find != mBinderMap.end())
        {
            SafeDelete(find->second);
            mBinderMap.erase(find);

            return true;
        }
        else
        {
            return false;
        }
    }
} // namespace core