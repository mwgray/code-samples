// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __EVENTDISPATCHER_H__
#define __EVENTDISPATCHER_H__

#include "Event.h"

#include "EventListenerList.h"

namespace core {

    class EventDispatcher
    {
    public:        
        virtual ~EventDispatcher();//{}

        // gets called for all events
        template <typename tListener> 
        void addEventListener(tListener* listener, void (tListener::*memberFunction)(const Event* event), bool removeAfterDispatch=false)
        {
            mWildcardListeners.addEventListener(listener, memberFunction, removeAfterDispatch);
        }

        template <typename tListener> 
        void removeEventListener(tListener* listener, void (tListener::*memberFunction)(const Event* event))
        {
            mWildcardListeners.removeEventListener(listener, memberFunction);
        }

        // TODO - should generalize this pattern to be a Function class
        template <typename tListener> 
        void addEventListener(const CompactString& eventType, tListener* listener, void (tListener::*memberFunction)(const Event* event), bool removeAfterDispatch=false)
        {
            mEventListenerMap[eventType].addEventListener(listener, memberFunction, removeAfterDispatch);
        }

        template <typename tListener> 
        void removeEventListener(const CompactString& eventType, tListener* listener, void (tListener::*memberFunction)(const Event* event))
        {
            EventListenerMap::iterator listenersIter = mEventListenerMap.find(eventType);

            if(listenersIter != mEventListenerMap.end())
            {
                EventListenerList& list = listenersIter->second;
                list.removeEventListener(listener, memberFunction);
            }
        }

         virtual void dispatchEvent(const Event& event);

    protected:
        typedef std::map<CompactString, EventListenerList> EventListenerMap;
        
        EventListenerList mWildcardListeners;
        EventListenerMap mEventListenerMap;        
    };

} // namespace core

#endif // __EVENTDISPATCHER_H__