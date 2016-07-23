// Copyright 2006-12 HumaNature Studios Inc.

#include "PhyreCorePch.h"

namespace core {

    Subscription::Subscription()
        : mIsSubscribedToAll(false) 
    {

    }

    Subscription::~Subscription()
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

    bool Subscription::IsSubscribedTo(const CompactStringRelease& categoryName, bool addToList)
    {
        // MEMORY_POOL("Subscription");

        bool isSubscribed = false;

        if(mIsSubscribedToAll)
        {
            isSubscribed = true;

            if(addToList)
            {
                addCategory(categoryName, true);
            }
        }
        else
        {
            SubscriptionMap::iterator i = mSubscriptionMap.find(categoryName);

            if(i == mSubscriptionMap.end())
            {
                isSubscribed = false;

                if(addToList)
                {
                    addCategory(categoryName, false);
                }
            }
            else
            {
                isSubscribed = i->second;
            }
        }

        return isSubscribed;
    }

    bool Subscription::ToggleCategory(const CompactStringRelease& categoryName)
    {
        SubscriptionMap::iterator i = mSubscriptionMap.find(categoryName);
        if(i != mSubscriptionMap.end())
        {
            i->second = !i->second;
            return true;
        }
        return false;
    }

    void Subscription::addCategory(const CompactStringRelease& categoryName, bool initialValue)
    {
        mSubscriptionMap[categoryName] = initialValue;
    }

} // namespace core