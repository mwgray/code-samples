// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __SCREENREPORTER_H__
#define __SCREENREPORTER_H__

// TODO - remove when script binding is in
#include "Input/IInputEventListener.h"

namespace core {

    class ConsoleLuaScript;

    class ScreenReporter
        : public Logger
        , public Singleton<ScreenReporter>
        , public IInputEventListener
    {
    public:
        ScreenReporter();

        void Initialize();        
        
        void Finalize();

        void bindFunctions(LuaContext* context);

        virtual ErrorBehavior logVargs(Severity severity, const CompactStringDebug& category, const char* message, va_list& vargs);

        virtual void handleInputEvent(unsigned int eventType, void* eventData);
    
    protected:
        SubscriptionMap::iterator mCurrentCategory;
        bool mEnabled;
        int mLineIndex;
        int mCategoryIndex;

        virtual void addCategory(const CompactStringRelease& categoryName, bool initialValue);

        void updateIndex();

        void clear();
        
        void writeLine(const char* text, int color);

        void toggleEnabled();

        void nextCategory(unsigned int callDepth = 0);

        void previousCategory(unsigned int callDepth = 0);

        friend class ScreenText;
        friend class ConsoleLuaScript;
    };

} // namespace core

#ifdef __HNS_LOGGING_ENABLED__

#define reportFatal(category, message, ...) core::ScreenReporter::Instance().fatal(category, message, ## __VA_ARGS__)
#define reportError(category, message, ...) core::ScreenReporter::Instance().error(category, message, ## __VA_ARGS__)
#define  reportWarn(category, message, ...) core::ScreenReporter::Instance().warn (category, message, ## __VA_ARGS__)
#define  reportInfo(category, message, ...) core::ScreenReporter::Instance().info (category, message, ## __VA_ARGS__)
#define reportDebug(category, message, ...) core::ScreenReporter::Instance().debug(category, message, ## __VA_ARGS__)
#define reportTrace(category, message, ...) core::ScreenReporter::Instance().trace(category, message, ## __VA_ARGS__)

#else // __HNS_LOGGING_ENABLED__

#define reportFatal(category, message, ...) DEAD_STRIP(category); DEAD_STRIP(message); DEAD_STRIP_PARAMS(__VA_ARGS__);
#define reportError(category, message, ...) DEAD_STRIP(category); DEAD_STRIP(message); DEAD_STRIP_PARAMS(__VA_ARGS__);
#define  reportWarn(category, message, ...) DEAD_STRIP(category); DEAD_STRIP(message); DEAD_STRIP_PARAMS(__VA_ARGS__);
#define  reportInfo(category, message, ...) DEAD_STRIP(category); DEAD_STRIP(message); DEAD_STRIP_PARAMS(__VA_ARGS__);
#define reportDebug(category, message, ...) DEAD_STRIP(category); DEAD_STRIP(message); DEAD_STRIP_PARAMS(__VA_ARGS__);
#define reportTrace(category, message, ...) DEAD_STRIP(category); DEAD_STRIP(message); DEAD_STRIP_PARAMS(__VA_ARGS__);

#endif // __HNS_LOGGING_ENABLED__

#endif // __SCREENREPORTER_H__