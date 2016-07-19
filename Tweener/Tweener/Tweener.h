// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __TWEENER_H__
#define __TWEENER_H__

#include "Tween.h"

namespace core {
    
    class TweenerImpl
    {
    public:
        // helper function to call a function after a given amount of time
        template <typename CallbackClass> 
        Tween& addTimedCallback(float time, CallbackClass* object, void (CallbackClass::*callback)()) 
        {
            return addTween().time(time).onComplete(object, callback);
        }

        // add a tween to the list, return it for setup using named parameter chain
        // example - this fades a color to white over 1 second, after a half second delay
        //   Tweener.addTween("FadeToWhite")
        //          .time(1.0f)
        //          .delay(0.5f)
        //          .type(EaseInOutQuint)
        //          .param(&color.r, 1.0)
        //          .param(&color.g, 1.0)
        //          .param(&color.b, 1.0)
        //          .onComplete(this, &Example::onFadeToWhiteComplete)
        Tween& addTween(const char* name="");

        // update all tweens
        void update(float deltaSeconds);

        // remove a single tween
        void removeTween(Tween* tween);

        // remove all tweens
        void removeAllTweens();

    protected:
        // holds all tweens
        typedef std::list<Tween*> TweenList;
        TweenList mTweens;
    };

    // expose global for easy access
    extern TweenerImpl Tweener;

} // namespace core

#endif // __TWEENER_H__