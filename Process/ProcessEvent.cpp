// Copyright 2006-12 HumaNature Studios Inc.

#include "CorePch.h"

#include "ProcessEvent.h"

namespace core {

    const CompactString ProcessEvent::PROCESS_BEGAN("processBegan");    
    const CompactString ProcessEvent::PROCESS_CONTINUE("processSubmitted");
    const CompactString ProcessEvent::PROCESS_ENDED("processEnded");

    ProcessEvent::ProcessEvent(const CompactString& type, Process& process) 
        : Event(type)
        , mProcess(process)
    {

    }

} // namespace core