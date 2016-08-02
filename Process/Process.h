// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "Event/EventDispatcher.h"

namespace core {

    class Process
        : public EventDispatcher
        , public MemoryObject
    {
    public:

        Process(const char* name=nullptr);

        virtual ~Process();

        bool hasBegun();
        
        bool hasEnded();

        void begin();

        void end(const char* additionalEvent=nullptr);

        void reset();

    protected:
        bool mBegun;
        bool mEnded;
        bool mDeleteOnEnd;
        std::string mName;

        virtual const char* getReportName();

        virtual void onBegin();

        virtual void onEnd();

    protected:
        void dispatch(const CompactString& event);

        friend class ProcessManager;
    };    

} // namespace core

#endif // __PROCESS_H__