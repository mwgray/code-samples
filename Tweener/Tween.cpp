// Copyright 2006-12 HumaNature Studios Inc.

#include "Tween.h"

#include "TweenCallback.h"
#include "Misc.h"

namespace core {

    Tween::Tween(const char* name) 
        : mName(nullptr)        
        , mDelay(0.0f)
        , mTime(0.0f)
        , mFinalTime(-1.0f)
        , OnStart(nullptr)
        , OnUpdate(nullptr)
        , OnComplete(nullptr)
        , equation(TweenEquation::getEquation(TweenType_Default))
    {
        if(name != nullptr)
        {
            mName = new std::string(name);
        }
    }

    Tween::~Tween()
    {
        SafeDelete(mName);
        SafeDelete(OnStart);
        SafeDelete(OnUpdate);
        SafeDelete(OnComplete);
    }

    Tween& Tween::time(float value)
    {
        mFinalTime = value;

        return *this;
    }

    Tween& Tween::delay(float value)
    {
        mDelay = value;
        mTime = -mDelay; 
        
        return *this;
    }

    Tween& Tween::type(TweenType value)
    {
        equation = TweenEquation::getEquation(value);

        return *this;
    }

    Tween& Tween::param(float* value, float target)
    {
        Param newParam;

        // store pointer, current, target values
        newParam.mValue = value;
        newParam.mStartingValue = *value;
        newParam.mTargetValue = target;

        // push the parameter
        params.push_back(newParam); 

        return *this;
    }

    Tween& Tween::restart()
    {
        mTime = -mDelay;
        
        return *this;
    }

    void Tween::update(float deltaSeconds)
    {
        mTime = MathUtil::min(mTime + deltaSeconds, mFinalTime);

        if (mTime < 0.0f)
        {
            return;
        }
        
        if(OnStart)
        {
            (*OnStart)();
            SafeDelete(OnStart);
        }

        bool complete = isComplete();

        for(auto i = params.begin(); i != params.end(); ++i)
        {
            auto p = *i;
            if(complete)
            {
                *p.mValue = p.mTargetValue;
            }
            else
            {
                *p.mValue = equation(mTime, p.mStartingValue, p.mTargetValue - p.mStartingValue, mFinalTime);
            }
        }

        if(OnUpdate)
        {
            (*OnUpdate)();
        }

        if(complete)
        {
            mFinalTime = -1.0f;
            if (OnComplete)
            {
                (*OnComplete)();
            }
        }
    }

    bool Tween::isComplete()
    {
        // compare with an epsilon value to avoid rounding issues
        return (mFinalTime - mTime) < 0.001f;
    }

} // namespace core