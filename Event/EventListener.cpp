// Copyright 2006-12 HumaNature Studios Inc.

#include "CorePch.h"

#include "EventListener.h"

namespace core {

    EventListener::EventListener(bool removeAfterDispatch) 
        : mRemoveAfterDispatch(removeAfterDispatch)
    {

    }

    EventListener::~EventListener()
    {

    }

} // namespace core