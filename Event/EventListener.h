// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __EVENTLISTENER_H__
#define __EVENTLISTENER_H__

namespace core {

    class Event;

    // internally used classes for EventDispatcher
    class EventListener
        : public MemoryObject
    {
    public:
        const bool mRemoveAfterDispatch;

        EventListener(bool removeAfterDispatch);

        virtual ~EventListener();

        virtual void operator()(const Event* event) = 0;

    protected:
        EventListener& operator=(const EventListener&);
    };

    template <class TClass> 
    class EventListenerTemplate : public EventListener
    {
    public:
        TClass* mObject;                         
        void (TClass::*mMemberFunction)(const Event* event);

        EventListenerTemplate(TClass* object, void (TClass::*memberFunction)(const Event* event), bool removeAfterDispatch)
            : EventListener(removeAfterDispatch)
            , mObject(object)
            , mMemberFunction(memberFunction)
        {

        };

        virtual void operator()(const Event* event)
        {
            (*mObject.*mMemberFunction)(event);
        };
    };

} // namespace core

#endif // __EVENTLISTENER_H__