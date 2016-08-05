// Copyright 2006-12 HumaNature Studios Inc.

#include "CorePch.h"

#include "ScreenReporter.h"

#include "Input/InputManager.h"
#include "Scripting/LuaContext.h"

DEFINE_SINGLETON_TYPE(core, ScreenReporter);

namespace core {
    ScreenReporter::ScreenReporter()
        : mCurrentCategory(mSubscriptionMap.begin())
        , mEnabled(false)
        , mLineIndex(0)
        , mCategoryIndex(0)
    {
    }

    void ScreenReporter::Initialize()
    {
        InputManager::Instance()->RegisterEventListener(this);
    }

    void ScreenReporter::Finalize()
    {
        InputManager::Instance()->UnregisterEventListener(this);
    }

    void ScreenReporter::bindFunctions(LuaContext* context)
    {
        context->bind("report", this, &ScreenReporter::SubscribeTo);
        context->bind("reportAll", this, &ScreenReporter::SubscribeToAll);
        context->bind("toggleReporter", this, &ScreenReporter::toggleEnabled);
    }

    Logger::ErrorBehavior ScreenReporter::logVargs(Severity severity, const CompactStringDebug& category, const char* message, va_list& vargs)
    {
        if(mEnabled && IsSubscribedTo(category))
        {
            if(mCurrentCategory == mSubscriptionMap.end())
            {
                mCurrentCategory = mSubscriptionMap.begin();
                mCategoryIndex = 1;
                for (;(mCurrentCategory != mSubscriptionMap.end()) && (category != mCurrentCategory->first); ++mCurrentCategory, ++mCategoryIndex) { }
            }

            if(category == mCurrentCategory->first)
            {
                // TODO - ugh, not the best here...  cleanup later
                LargeStaticCharBuffer largeOutputBuffer;
                char * outputBuffer = largeOutputBuffer.getBuffer();
                vsprintf(outputBuffer, message, vargs);

                writeLine(outputBuffer, severityColors[(int)severity]);
            }
        }

        return kContinue;
    }

    void ScreenReporter::addCategory(const CompactStringRelease& categoryName, bool initialValue)
    {
        Subscription::addCategory(categoryName, initialValue);

        updateIndex();
    }

    void ScreenReporter::updateIndex()
    {
        mCategoryIndex = 1;
        for (auto i = mSubscriptionMap.begin(); i != mSubscriptionMap.end() && i != mCurrentCategory; ++i, ++mCategoryIndex)
        {
        }
    }

    void ScreenReporter::clear()
    {
        if(mEnabled && !mSubscriptionMap.empty() && mCurrentCategory != mSubscriptionMap.end())
        {
            screenTextColor(0, 0, 0, "[%d/%d]", mCategoryIndex, mSubscriptionMap.size());
        }

        mLineIndex = 1;
    }

    void ScreenReporter::writeLine(const char* text, int color)
    {
        screenTextLineColor(0, mLineIndex++, color, text);
    }

    void ScreenReporter::toggleEnabled()
    {
        mEnabled = !mEnabled;
    }

    void ScreenReporter::nextCategory(unsigned int callDepth)
    {
        if(!mSubscriptionMap.empty() && mCurrentCategory != mSubscriptionMap.end())
        {
            ++mCurrentCategory;

            if(mCurrentCategory == mSubscriptionMap.end())
            {
                mCurrentCategory = mSubscriptionMap.begin();
            }

            if(!mCurrentCategory->second && callDepth < mSubscriptionMap.size())
            {
                // not subscribed, keep iterating
                nextCategory(callDepth++);
            }
            else
            {
                updateIndex();
            }
        }
    }

    void ScreenReporter::previousCategory(unsigned int callDepth)
    {
        if(!mSubscriptionMap.empty() && mCurrentCategory != mSubscriptionMap.end())
        {
            if(mCurrentCategory == mSubscriptionMap.begin())
            {
                mCurrentCategory = mSubscriptionMap.end();
                mCategoryIndex = mSubscriptionMap.size() + 1;
            }

            --mCurrentCategory;
            --mCategoryIndex;

            if(!mCurrentCategory->second && callDepth < mSubscriptionMap.size())
            {
                // not subscribed, keep iterating
                previousCategory(callDepth++);
            }
            else
            {
                updateIndex();
            }
        }
    }

    void ScreenReporter::handleInputEvent(unsigned int eventType, void* eventData)
    {
        switch (eventType)
        {
        case kKeyUp:
            {
                KeyInfo* keyEvent = (KeyInfo*)eventData;
                switch (keyEvent->getKeyID())
                {
                case PInputBase::InputChannel_Key_Left:
                case PInputBase::InputChannel_Button_Left:
                    previousCategory();
                    break;
                case PInputBase::InputChannel_Key_Right:
                case PInputBase::InputChannel_Button_Right:
                    nextCategory();
                    break;
                case PInputBase::InputChannel_Key_Escape:
                case PInputBase::InputChannel_Button_Start:
                    toggleEnabled();
                    break;
                }
            }
            break;
        }
    }
} // namespace core