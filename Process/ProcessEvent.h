// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __PROCESSEVENT_H__
#define __PROCESSEVENT_H__

#include "Event/Event.h"

namespace core {

    class Process;

    class ProcessEvent
        : public Event
    {
    public:
        static const CompactString PROCESS_BEGAN;
        static const CompactString PROCESS_CONTINUE; // indicates the next process can be launched
        static const CompactString PROCESS_ENDED;
        
        const Process& mProcess;

        ProcessEvent(const CompactString& type, Process& process);

    protected:
        ProcessEvent& operator=(const ProcessEvent&);
    };

} // namespace core

#endif // __PROCESSEVENT_H__