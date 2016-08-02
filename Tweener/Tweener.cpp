// Copyright 2006-12 HumaNature Studios Inc.

#include "Tweener.h"

#include "Misc.h"

namespace core {

    TweenerImpl Tweener;

    void TweenerImpl::update(float deltaSeconds)
    {
        // iterate through all tweens
        for(auto i = mTweens.begin(); i != mTweens.end();)
        {
            auto tween = *i;

            if(tween->isRemoved())
            {
                // if the tween is removed, erase it and update the iterator
                SafeDelete(tween);
                i = mTweens.erase(i);
            }
            else
            {
                // otherwise update the tween and increment the iterator
                tween->update(deltaSeconds);
                ++i;
            }
        }
    }

    Tween& TweenerImpl::addTween(const char* name)
    {
        // allocate the tween
        auto tween = new Tween(name);

        // add to the list of all tweens
        mTweens.push_back(tween);

        // return a reference to the tween
        return *tween;
    }

    void TweenerImpl::removeTween(Tween* tween)
    {
        // find the tween
        auto i = std::find(mTweens.begin(), mTweens.end(), tween);

        // mark it as removed, will be cleaned up during the next update
        if (i != mTweens.end())
        {
            tween->remove();
        }
    }

    void TweenerImpl::removeAllTweens()
    {
        // iterate through all the tweens and delete them
        for(auto i = mTweens.begin(); i != mTweens.end(); ++i)
        {
            SafeDelete(*i);
        }

        // clear the list
        mTweens.clear();
    }
} // namespace core