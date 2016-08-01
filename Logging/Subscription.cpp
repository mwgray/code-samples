// Copyright 2006-12 HumaNature Studios Inc.

#include "PhyreCorePch.h"

namespace core {

    Subscription::Subscription()
        : mIsSubscribedToAll(false) 
    {

    }

    void Subscription::SubscribeTo(const CompactStringRelease& categoryName)
    {
        mSubscriptionMap[categoryName] = true;
    }

    void Subscription::SubscribeToAll()
    {
        mIsSubscribedToAll = true;
    }

    void Subscription::UnsubscribeFrom(const CompactStringRelease& categoryName)
    {
        mSubscriptionMap[categoryName] = false;
    }

    bool Subscription::IsSubscribedTo(const CompactStringRelease& categoryName)
    {
        bool isSubscribed = false;

        if(mIsSubscribedToAll)
        {
            isSubscribed = true;
        }
        else
        {
            SubscriptionMap::iterator i = mSubscriptionMap.find(categoryName);

            if(i == mSubscriptionMap.end())
            {
                isSubscribed = false;
            }
            else
            {
                isSubscribed = i->second;
            }
        }

        return isSubscribed;
    }

    void Subscription::ToggleCategory(const CompactStringRelease& categoryName)
    {
        SubscriptionMap::iterator i = mSubscriptionMap.find(categoryName);
        if(i != mSubscriptionMap.end())
        {
            i->second = !i->second;
        }
    }

} // namespace core