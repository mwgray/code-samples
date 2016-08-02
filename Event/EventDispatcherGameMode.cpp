// Copyright 2006-12 HumaNature Studios Inc.

#include "CorePch.h"

#include "EventDispatcher.h"
#include "GameMode/GameMode.h"

namespace core {

    //// TEST PROGRAM ///////////////////////////////////////////////////////////

#define RUNS 50 * 1000 * 1000

#define TIME(x) float(x) / CLOCKS_PER_SEC

    //// C FUNCTION /////////////////////////////////////////////////////////////

    class Base
    {
    public:
        int Run(int i)
        {
            return m = i;
        }

    protected:
        int m;
    };

    static Base obj;
    static inline int DummyFunc(int i)
    {
        return obj.Run(i);
    }

    //// PTR TO MEMBER //////////////////////////////////////////////////////////

    class PtrBase
    {
    protected:
        typedef int (PtrBase::*FuncPtr)(int);

    public:
        PtrBase()
        {
            m = 0;
            m_pFunc = &PtrBase::Function;
        }

        int Run(int i)
        {
            return (this->*m_pFunc)(i);
        }

        FuncPtr m_pFunc;
    protected:
        int m;

    private:
        int Function(int i)
        {
            return m = i;
        }
    };

    class PtrDerived1 : public PtrBase
    {
    public:
        PtrDerived1()
        {
            m = 0;
            m_pFunc = (FuncPtr)&PtrDerived1::DerivedFunc;
        }

    private:
        int DerivedFunc(int i)
        {
            return m = i;
        }
    };

    //// VIRTUAL INHERITANCE ////////////////////////////////////////////////////

    class VirtBase
    {
    public:
        VirtBase()
        {
            m = 0;
        }

        virtual ~VirtBase() {}

        virtual int Run(int i) =0;

    protected:
        int m;
    };

    class VirtDerived1 : public VirtBase
    {
    public:
        virtual int Run(int i);
        int m1;
    };

    int VirtDerived1::Run(int i)
    {
        return m1 = i;
    }

    class VirtDerived2 : public VirtDerived1
    {
    public:
        virtual int Run(int i)
        {
            return m2 = i;
        }
        int m2;
    };

    class VirtDerived3 : public VirtDerived2
    {
    public:
        virtual int Run(int i)
        {
            return m3 = i;
        }
        int m3;
    };

    //// STATIC FUNCTION ////////////////////////////////////////////////////////

    class StaticBase
    {
    public:
        StaticBase()
        {
            m = 0;
            p = this;
        }

        static int StaticRun(int i)
        {
            return p->Run(i);
        }

    protected:
        int Run(int i)
        {
            return m = i;
        }

        static StaticBase* p;
        int m;
    };

    StaticBase* StaticBase::p = 0;

    //// FUNCTOR ////////////////////////////////////////////////////////////////

    class FunctorBase
    {
    public:
        FunctorBase()
        {
            m = 0;
        }

        int Run(int i)
        {
            return m = i;
        }

    protected:
        int m;
    };

    class TFunctor
    {
    public:

        // two possible functions to call member function. virtual cause derived
        // classes will use a pointer to an object and a pointer to a member function
        // to make the function call
        virtual ~TFunctor() {};

        virtual void operator()(int)=0;  // call using operator
        virtual void Call(int)=0;        // call using function
    };


    // derived template class
    template <class TClass> class TSpecificFunctor : public TFunctor
    {
    private:
        int (TClass::*fpt)(int);   // pointer to member function
        TClass* pt2Object;                  // pointer to object

    public:

        // constructor - takes pointer to an object and pointer to a member and stores
        // them in two private variables
        TSpecificFunctor(TClass* _pt2Object, int (TClass::*_fpt)(int))
        {
            pt2Object = _pt2Object;
            fpt = _fpt;
        };

        // override operator "()"
        virtual void operator()(int string)
        {
            (*pt2Object.*fpt)(string);
        };              // execute member function

        // override function "Call"
        virtual void Call(int string)
        {
            (*pt2Object.*fpt)(string);
        };             // execute member function
    };

    EventDispatcher e;

    class TestClassA
    {
    public:
        void handleEvent(const Event*)
        {

        }

        void handleEventLog(const Event*)
        {
            logWarn("Event", "Handled A!");
        }
    };

    class TestClassB
    {
    public:
        void handleEvent(const Event*)
        {

        }

        void handleEventLog(const Event*)
        {
            logWarn("Event", "Handled B!");
        }
    };

    class TestClassC
    {
    public:
        void handleEvent(const Event*)
        {

        }

        void handleEventLog(const Event*)
        {
            logWarn("Event", "Handled C!");
        }
    };

    class EventDispatcherGameMode
        : public GameMode
    {
    public:
        virtual void Preload(PropertyObject* properties)
        {
            UNUSED_PARAM(properties);
            TestClassA a;
            TestClassB b;
            TestClassC c;

            e.addEventListener("Event", &a, &TestClassA::handleEventLog);
            e.addEventListener("Event", &b, &TestClassB::handleEventLog);
            e.addEventListener("Event", &c, &TestClassC::handleEventLog, true);

            Event event("Event");

            e.dispatchEvent(event);
            e.dispatchEvent(event);

            e.removeEventListener("Event", &b, &TestClassB::handleEventLog);
            e.removeEventListener("Event", &a, &TestClassA::handleEventLog);

            e.dispatchEvent(event);

            profile();
        }

        void profile()
        {
            int i;
            clock_t c;

            logWarn("Test", "Test starts...");

            //----- C Function --------------------------------

            c = clock();
            int (*pDummyFunc)(int) = DummyFunc;
            for(i = 0; i <= RUNS; i++)
            {
                //DummyFunc(i); is much faster if you make it inline
                pDummyFunc(i);

                if(i > 0 && i % 10000000 == 0)
                {
                    logWarn("Test", "C-Function @%d0Mil %2.2f", i/10000000, TIME(clock()-c));
                }
            }            

            //----- Static Function ------------------------

            StaticBase sb;
            int (*pStaticFunc)(int) = StaticBase::StaticRun;
            c = clock();
            for(i = 0; i <= RUNS; i++)
            {
                pStaticFunc(i);
                if(i > 0 && i % 10000000 == 0)
                {
                    logWarn("Test", "Static Function @%d0Mil %2.2f", i/10000000, TIME(clock()-c));
                }
            }

            //----- Virtual Function --------------------------

            VirtBase* pv = new VirtDerived1();
            c = clock();
            for(i = 0; i <= RUNS; i++)
            {
                pv->Run(i);
                if(i > 0 && i % 10000000 == 0)
                {
                    logWarn("Test", "Virtual Function @%d0Mil %2.2f", i/10000000, TIME(clock()-c));
                }
            }
            delete pv;

            //----- Ptr to Member Function --------------------

            PtrBase* pb = new PtrDerived1();
            c = clock();
            for(i = 0; i <= RUNS; i++)
            {
                // no difference if called through a pointer
                //(pb->*(pb->m_pFunc))(i);
                pb->Run(i);

                if(i > 0 && i % 10000000 == 0)
                {
                    logWarn("Test", "Pointer To Member Function @%d0Mil %2.2f", i/10000000, TIME(clock()-c));
                }
            }
            delete pb;

            //----- Bound Pointer(stl) -----------------------------

            FunctorBase fb;

            //c = clock();
            // MIKE 
            //for(i = 0; i <= RUNS; i++)
            //{
            //    std::bind1st(std::mem_fun(&FunctorBase::Run), &fb)(i);
            //    if(i > 0 && i % 10000000 == 0)
            //    {
            //        logWarn("Test", "STL Bound Function @%d0Mil %2.2f", i/10000000, TIME(clock()-c));
            //    }
            //}

            //----- Bound Pointer templates -----------------------------
            TSpecificFunctor<FunctorBase> specFuncA(&fb, &FunctorBase::Run);

            c = clock();
            for(i = 0; i <= RUNS; i++)
            {
                specFuncA(i);

                if(i > 0 && i % 10000000 == 0)
                {
                    logWarn("Test", "Templated Functor @%d0Mil %2.2f", i/10000000, TIME(clock()-c));
                }
            }

            //----- Event Dispatcher -----------------------------

            TestClassA t;

            EventDispatcher e;

            e.addEventListener("Event", &t, &TestClassA::handleEvent);

            Event event("Event");

            c = clock();
            for(i = 0; i <= RUNS; i++)
            {
                e.dispatchEvent(event);

                if(i > 0 && i % 1000000 == 0)
                {
                    logWarn("Test", "Event Disptacter @%dMil %2.2f", i/1000000, TIME(clock()-c));
                }
            }

            // return 0;
        }
    };

    REGISTER_FACTORY_CLASS_NS(core, EventDispatcherGameMode, GameMode);

} // namespace core

DEFINE_TYPE_NS(core, TestClassA);
DEFINE_TYPE_NS(core, TestClassB);
DEFINE_TYPE_NS(core, TestClassC);
