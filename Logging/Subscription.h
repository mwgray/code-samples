// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __SUBSCRIPTION_H__
#define __SUBSCRIPTION_H__

namespace core  {

class Subscription
{
public:
    bool mIsSubscribedToAll;

    Subscription();

    virtual ~Subscription();

    void SubscribeTo(const CompactStringRelease& categoryName);

    void SubscribeToAll();

    void UnsubscribeFrom(const CompactStringRelease& categoryName);

    bool ToggleCategory(const CompactStringRelease& categoryName);

    bool IsSubscribedTo(const CompactStringRelease& categoryName, bool addToList=false);

protected:
    typedef std::map<CompactStringRelease, bool> SubscriptionMap;
    SubscriptionMap mSubscriptionMap;

    virtual void addCategory(const CompactStringRelease& categoryName, bool initialValue);
};

} // namespace core

#endif // __SUBSCRIPTION_H__