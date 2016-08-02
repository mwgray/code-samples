// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __TWEENCALLBACK_H__
#define __TWEENCALLBACK_H__

namespace core {

    // template-less base class, so different types can be stored/called
    class TweenCallback 
    {
    public:
        TweenCallback();

        virtual ~TweenCallback() = 0;

        virtual void operator()() = 0;

    protected:
        void* mObjectAddress;

        friend class Tween;
    };

    // template version of callback class, allows for quick binding of member functions
    template <class CallbackClass>
    class TweenCallbackTemplate
        : public TweenCallback
    {
    public:
        TweenCallbackTemplate()
            : mObject(nullptr)
            , mMemberFunction(nullptr)
        {

        }

        // set the callback to a member function of a supplied object
        void set(CallbackClass *object, void (CallbackClass::*memberFunction)())
        {
            mObjectAddress = object;
            mObject = object;
            mMemberFunction = memberFunction;
        }

        // call the function
        virtual void operator()()
        {
            if(mMemberFunction) {
                (*mObject.*mMemberFunction)();
            }
        }

    protected:
        // pointer to the object on which to call the member function
        CallbackClass* mObject;                         

        // pointer to the member function
        void (CallbackClass::*mMemberFunction)();

        // hide assignment
        TweenCallbackTemplate& operator=(TweenCallbackTemplate&);
    };

} // namespace core

#endif // __TWEENCALLBACK_H__