// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __TWEEN_H__
#define __TWEEN_H__

#include "TweenType.h"
#include "TweenEquation.h"
#include "TweenCallback.h"

#include <string>
#include <list>

namespace core {

    class TweenCallback;

    class Tween
    {
    public:
        // construct a tween with a name
        Tween(const char* name);
        ~Tween();        

        // sets the time in seconds for the tween to complete
        Tween& time(float value);

        // sets the delay time in seconds for the tween to complete
        Tween& delay(float value);

        // sets the TweenType of the tween
        Tween& type(TweenType value);

        // sets the value to change and the target to seek to.  uses the value when call as the starting value
        Tween& param(float* value, float target);

        // sets time to 0
        Tween& restart();

        // sets the onStart callback, which will be called when the tween starts, usually
        template <typename CallbackClass> 
        Tween& onStart(CallbackClass* object, void (CallbackClass::*callback)()) 
        {
            return setCallback(OnStart, object, callback);
        }

        // sets the onUpdate callback, which will be called each time the tween updates, usually many times
        template <typename CallbackClass> 
        Tween& onUpdate(CallbackClass* object, void (CallbackClass::*callback)()) 
        { 
            return setCallback(OnUpdate, object, callback);
        }

        // sets the onComplete callback, which will be called each time the tween updates, usually once
        template <typename CallbackClass> 
        Tween& onComplete(CallbackClass* object, void (CallbackClass::*callback)()) 
        {
            return setCallback(OnComplete, object, callback);
        }

        // updates the tween's time/values
        void update(float deltaSeconds);

        // returns true if the tween has finished
        bool isComplete();

        // removes the tween from the system
        void remove()
        {
            mFinalTime = -1.0f;
        }

        // returns true if the tween has been removed
        bool isRemoved() 
        { 
            return mFinalTime < 0.0f; 
        }

        // returns the name of the tween
        const char* getName() const
        {
            return mName ? mName->c_str() : "unnamed";
        }

    protected:
        // structure to track the value to change, and the starting/target value
        struct Param
        {
            float* mValue;
            float mStartingValue;
            float mTargetValue;
        };

        // name of the tween
        std::string* mName;

        // how long until the tween starts
        float mDelay;

        // the current time of the tween
        float mTime;

        // the final time of the tween, when mTime >= mFinaltime, it is complete
        float mFinalTime;

        // callbacks for Start, Update, Complete
        TweenCallback* OnStart;
        TweenCallback* OnUpdate;
        TweenCallback* OnComplete;

        // the TweenEquation used to update the tweened parameters
        TweenEquation::Signature equation;

        // list of paramters to update, there can be multiple ones tie to the same tween, ie, x, y, z or r,b,g,a
        typedef std::list<Param> ParamList;
        ParamList params;

        // templated function to set the callback function via a functor
        template <typename CallbackClass>
        Tween& setCallback(TweenCallback *&tweenCallback, CallbackClass *object, void (CallbackClass::*callback)())
        {
            if(tweenCallback && tweenCallback->mObjectAddress != (void*)object)
            {
                // there was a previously set callback, delete it
                delete tweenCallback;
            }

            if(tweenCallback == nullptr)
            {
                // create a new callback for a given class
                tweenCallback = new TweenCallbackTemplate<CallbackClass>();
            }

            // set the callback
            TweenCallbackTemplate<CallbackClass>* concreteCallbackClass = nullptr;
            concreteCallbackClass = static_cast <TweenCallbackTemplate<CallbackClass>* >(tweenCallback);
            concreteCallbackClass->set(object, callback);

            return *this;
        }

        friend class TweenerImpl;
    };

} // namespace core

#endif // __TWEEN_H__