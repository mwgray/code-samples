// Copyright 2006-12 HumaNature Studios Inc.

#include "CorePch.h"

#include "Event.h"

namespace core {

    REGISTER_PARENT_NS(core, Event, Castable);

    Event::Event(const CompactString& type)
        : mType(type) 
    {
        mClassInfo = &CastInfo::GetCastInfo<Event>();
        mClassInfo->setParent(&CastInfo::GetCastInfo<Castable>());
        mClassInfo->verify();
    }

} // namespace core
