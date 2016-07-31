// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __GAMELOGGER_H__
#define __GAMELOGGER_H__

namespace core {

    class GameLogger
        : public ConsoleLogger
        , public Singleton<GameLogger>
    {
    public:
        OVERRIDE_SINGLETON(GameLogger);

        bool mAllowModal;
        
        GameLogger();

        virtual ~GameLogger();

        virtual void bindFunctions(LuaFunctionBinder* binder);
        
        virtual ErrorBehavior handlePriority(Priority priority, const CompactStringDebug& category, LargeStaticCharBuffer& buffer, int minDepth);
    };


} // namespace core

#endif // __GAMELOGGER_H__