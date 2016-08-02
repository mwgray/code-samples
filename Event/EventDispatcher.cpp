// Copyright 2006-12 HumaNature Studios Inc.

#include "CorePch.h"

#include "EventDispatcher.h"
#include "Scaleform/ExternalInterfaceEvent.h"

namespace core {

    void EventDispatcher::dispatchEvent(const Event& event)
    {
        if (!mEventListenerMap.empty())
        {
            EventListenerMap::iterator i = mEventListenerMap.find(event.mType);
            if(i != mEventListenerMap.end())
            {
                i->second.dispatchEvent(event);
            }
        }

        mWildcardListeners.dispatchEvent(event);
    }

    EventDispatcher::~EventDispatcher()
    {
        DelayedEventDispatcher::Instance().onDeleteDispatcher(this);
    }

} // namespace core