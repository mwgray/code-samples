// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __EVENT_H__
#define __EVENT_H__

namespace core {

    class Event
        : public MemoryObject
        , public Castable
    {
    public:
        CompactString mType;

        // forceNonCastable is specifically for statically generated events
        // note, these will complain bitterly if type_cast is used on these
        Event(const CompactString& type);
    };

} // namespace core

#endif // __EVENT_H__