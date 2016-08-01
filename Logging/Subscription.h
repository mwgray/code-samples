// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __SUBSCRIPTION_H__
#define __SUBSCRIPTION_H__

namespace core  {

class Subscription
{
public:
    Subscription();

    void SubscribeTo(const CompactStringRelease& categoryName);

    void SubscribeToAll();

    void UnsubscribeFrom(const CompactStringRelease& categoryName);

    void ToggleCategory(const CompactStringRelease& categoryName);

    bool IsSubscribedTo(const CompactStringRelease& categoryName);

protected:
    typedef std::map<CompactStringRelease, bool> SubscriptionMap;
    SubscriptionMap mSubscriptionMap;

    bool mIsSubscribedToAll;
};

} // namespace core

#endif // __SUBSCRIPTION_H__