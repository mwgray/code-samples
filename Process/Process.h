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

        Process();

        virtual ~Process();

        bool hasBegun();
        
        bool hasEnded();

        bool isAsnycProcess() const;

        void begin();

        void end(const char* additionalEvent=nullptr);

        void reset();

    protected:
        bool mBegun;
        bool mEnded;
        bool mDeleteOnEnd;
        bool mIsAsyncProcess;

        virtual const char* getReportName();

        virtual void onBegin();

        virtual void onEnd();

    protected:
        void dispatch(const CompactString& event);

        friend class ProcessManager;
    };    

} // namespace core

#endif // __PROCESS_H__