// Copyright 2006-12 HumaNature Studios Inc.

#include "CorePch.h"

#include "EventListenerList.h"

// needed for ~EventListener
#include "EventListener.h"

namespace core {
    EventListenerList::~EventListenerList()
    {
        for(iterator i = begin(); i != end(); ++i)
        {
            SafeDelete(*i);
        }
    }

    void EventListenerList::dispatchEvent(const Event& event)
    {
        if (empty())
            return;

        // TODO - stl sooo slow for this...
        //        if there's a performance issue, make our own containers,
        //        or replace with fast stl impl
        // reverse direction for when remove is called during callback
        for (iterator i = begin(); i != end();)
        {
            EventListener* listener = *i;

            // cache this, since the listener could get deleted in a callback,
            // causing the pointer to result in true
            bool removeAfter = listener->mRemoveAfterDispatch;
            if(removeAfter)
            {
                i = erase(i);
            }
            else
            {
                ++i;
            }

            (*listener)(const_cast<Event*>(&event));

            if(removeAfter)
            {
                SafeDelete(listener);
            }
        }
    }
} // namespace core