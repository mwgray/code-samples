// Copyright 2006-12 HumaNature Studios Inc.

#include "CorePch.h"

#include "GameLogger.h"

#include "Debug/Dialog/ModalDialogBox.h"
#include "Save/SaveLoadManager.h"
#include "Scripting/LuaFunctionBinder.h"

DEFINE_SINGLETON_TYPE(core, GameLogger);

namespace {
//    bool gInErrorSave = false;
}

namespace core {

    GameLogger::GameLogger()
        : mAllowModal(true)
    {
        OverrideSingleton<GameLogger, ConsoleLogger>(this);
#ifdef __HNS_WIN32__
        if (AttachConsole(ATTACH_PARENT_PROCESS))
        {
            mAllowModal = false;
        }
        else
        {
            // If we want to not show the ignore all window, uncomment
            // mAllowModal = !IsDebuggerPresent();
        }
#endif
    }

    GameLogger::~GameLogger()
    {
    }

    void GameLogger::bindFunctions(LuaFunctionBinder* binder)
    {
        binder->bind("log", this, &GameLogger::SubscribeTo);
        binder->bind("logAll", this, &GameLogger::SubscribeToAll);
        binder->bind("ignore", this, &GameLogger::ignore);
    }

    Logger::ErrorBehavior GameLogger::handlePriority(Priority priority, const CompactStringDebug& category, LargeStaticCharBuffer& buffer, int minDepth)
    {
//         if ((priority <= kError) && !gInErrorSave)
//         {
//             gInErrorSave = true;
// 
//             if (SaveLoadManager::InstancePtr())
//             {
//                 SaveLoadManager::Instance().startSave(kSaveTypeDebug);
//             }
//             gInErrorSave = false;
//         }

        if(mAllowModal && priority <= kError)
        {
            std::string title = Reflect.toEnumString(priority);
            title += ": ";
            title += category.c_str();

            int dialogResult = ModalDialogBox::show(title.c_str(), buffer.getBuffer(), BuildPlatform::IsDebuggerPresent() ? "&Break" : "&Exit", "&Ignore All", "&Continue");

            return ErrorBehavior(dialogResult);
        }
        else
        {
            return ConsoleLogger::handlePriority(priority, category, buffer, minDepth + 1);
        }
    }

} // namespace core