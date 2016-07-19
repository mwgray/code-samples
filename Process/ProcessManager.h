// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __PROCESSMANAGER_H__
#define __PROCESSMANAGER_H__

#include "ProcessTypes.h"

namespace core {

    class Event;

    class ProcessManager
        : public Singleton<ProcessManager>
    {
    public:
        bool mReportProcesses;

        ProcessManager();
        ~ProcessManager();

        void update(float deltaSeconds);

        void initialize();
        void finalize();

    protected:        
        ProcessList mRunningProcesses;
        ProcessList mDeadProcesses;

        // TODO - pull these up to a common list helper class?
        static void addProcessToList(Process* process, ProcessList& processList);

        static void removeProcessFromList(Process* process, ProcessList& processList);

        void addProcess(Process* process);

        void removeProcess(Process* process);

        friend class Process;
    };

} // namespace core

#endif // __PROCESSMANAGER_H__