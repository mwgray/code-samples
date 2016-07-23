// Copyright 2006-13 HumaNature Studios Inc.

#ifndef __SINGLETONLIFETIMEPOLICY_H__
#define __SINGLETONLIFETIMEPOLICY_H__

namespace core {

    class SingletonLifetimePolicy
    {
    public:
        typedef void (*AtExitFunction)();

        template <class T>
        struct DestroyAtExit
        {
            static void ScheduleDestruction(T*, AtExitFunction pFun)
            { 
                std::atexit(pFun); 
            }

            static void OnDeadReference()
            { 
                // logError("Singleton", "Dead Reference Detected"); 
                PHYRE_ASSERT(0);                
            }
        };

        //////////////////////////////////////////////////////////////////////////

        template <class T>
        struct DoNotDestroy
        {
            static void ScheduleDestruction(T*, AtExitFunction)
            {
                // do nothing
            }

            static void OnDeadReference()
            {
                // do nothing
            }
        };

        //////////////////////////////////////////////////////////////////////////

        template <class T>
        class ExplicitlyDestroy
        {
        public:
            static void ScheduleDestruction(T*, AtExitFunction pFun)
            {
                static bool firstPass = true;
                sIsDead = false;
                sDeleter = pFun;
                if (firstPass || sNeedCallback)
                {
                    std::atexit(atexitCallback);
                    firstPass = false;
                    sNeedCallback = false;
                }
            }

            static void OnDeadReference()
            { 
            }
            
            ///  delete singleton object manually
            static void Destroy()
            {
                if (sIsDead)
                {
                    return;
                }

                sIsDead = true;

                sDeleter();
            }

        protected:
            static AtExitFunction sDeleter;
            static bool sIsDead;
            static bool sNeedCallback;

            static void atexitCallback()
            {
                sNeedCallback = true;

                Destroy();
            }
        };
    };

    template <class T>
    SingletonLifetimePolicy::AtExitFunction SingletonLifetimePolicy::ExplicitlyDestroy<T>::sDeleter = 0;

    template <class T>
    bool SingletonLifetimePolicy::ExplicitlyDestroy<T>::sIsDead = true;

    template <class T>
    bool SingletonLifetimePolicy::ExplicitlyDestroy<T>::sNeedCallback = true;

} // namespace core

#endif // __SINGLETONLIFETIMEPOLICY_H__