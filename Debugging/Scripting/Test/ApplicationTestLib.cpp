// Copyright 2006-13 HumaNature Studios Inc.

#include "CorePch.h"

#include "LuaTestLib.h"

#include "Application/Application.h"
#include "Tweener/Tweener.h"

namespace core {

    // To test, use commandline option /test TestScene "'ert'"

    class ApplicationTestLib
        : public LuaTestLib
    {
    public:
        ApplicationTestLib() 
            : LuaTestLib()
            , mWaitKeyID(-1)
        {

        }

        virtual void bind(LuaContext* luaBinding)
        {
            LuaTestLib::bind(luaBinding);

            // attach bindings to script            
            mLuaBinding->bind("harness.setTimeMultiplier", this, &ApplicationTestLib::setTimeMultiplier);
            mLuaBinding->bind("harness.shutdown", this, &ApplicationTestLib::shutdown);
            mLuaBinding->bind("harness.fail", this, &ApplicationTestLib::fail);
            mLuaBinding->bind("harness.wait", this, &ApplicationTestLib::wait);
            mLuaBinding->bind("harness.waitForKey", this, &ApplicationTestLib::waitForKey);
        }
    protected:
        void setTimeMultiplier(float multiplier)
        {
            Application::Instance().mElapsedTimeMultiplier = multiplier;
        }

        void shutdown()
        {
            log("Application shutting down");
#ifdef _WIN32
            PostMessage(PWindowWin32::GetHWND(), WM_CLOSE, 0, 0);
#else
            Application::Instance().requestQuit();
#endif
        }

        void wait(float time)
        {
            log("Waiting %2.2f seconds", time);

            Tweener.addTimedCallback(time, this, &ApplicationTestLib::onWaitComplete);
        }

        void onWaitComplete()
        {
            log("Done waiting!", time);

            sendEvent("waitComplete");
        }

        int mWaitKeyID;
        void waitForKey(int keyID)
        {
            mWaitKeyID = keyID;
        }

        virtual void update(float deltaSeconds) 
        {
            LuaTestLib::update(deltaSeconds);
            if (mWaitKeyID != -1)
            {
                // check the key state:
                PApplication& app = Application::Instance();
                if (app.isKeyDown((PInput::PInputChannel)mWaitKeyID))
                {
                    onWaitForKeyComplete();
                }
            }
        }

        void onWaitForKeyComplete()
        {
            log("Done waiting for key!");
            mWaitKeyID = -1;
            sendEvent("waitForKeyComplete");
        }

        void fail(const char* reason)
        {
            logFatal("Test", "Script returned error: %s", reason);
        }
    };

    REGISTER_FACTORY_CLASS_NS(core, ApplicationTestLib, LuaTestLib);

} // namespace core