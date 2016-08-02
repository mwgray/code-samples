// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __EVENTLISTENERLIST_H__
#define __EVENTLISTENERLIST_H__

#include "EventListener.h"

namespace core {

    class Event;

    class EventListenerList
        : public std::list<EventListener*>
    {
    public:
        ~EventListenerList();

        // gets called for all events
        template <typename tListener> 
        void addEventListener(tListener* listener, void (tListener::*memberFunction)(const Event* event), bool removeAfterDispatch=false)
        {
            push_back(HNS_NEW_TAGGED(EventListenerTemplate<tListener>, GetCoreTypeName<tListener>())(listener, memberFunction, removeAfterDispatch));
        }

        template <typename tListener> 
        void removeEventListener(tListener* listener, void (tListener::*memberFunction)(const Event* event))
        {
            EventListenerList::iterator iter = begin();
            for(; iter != end();)
            {
                EventListenerTemplate<tListener>* listenerDownCast = static_cast<EventListenerTemplate<tListener>*>(*iter);
                if(listenerDownCast->mObject == listener
                    && listenerDownCast->mMemberFunction == memberFunction)
                {
                    SafeDelete(*iter);

                    iter = erase(iter);
                }
                else
                {
                    ++iter;
                }
            }
        }

        void dispatchEvent(const Event& event);
    };

} // namespace core

#endif // __EVENTLISTENERLIST_H__