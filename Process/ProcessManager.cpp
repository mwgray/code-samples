// Copyright 2006-12 HumaNature Studios Inc.

#include "CorePch.h"

#include "ProcessManager.h"

#include "Process.h"
#include "ProcessEvent.h"

DEFINE_SINGLETON_TYPE(core, ProcessManager);

namespace core {

    ProcessManager::ProcessManager() 
        : mReportProcesses(false)
    {
        
    }

    ProcessManager::~ProcessManager()
    {
    }

    void ProcessManager::initialize()
    {
        Bind::Member(mReportProcesses, this, "TrackProcesses")
            .comment("Enables/Disables tracking of processes.")
            .isBootOnly(false);
    }

    void ProcessManager::finalize()
    {
        Unbind::Host(this);
    }

    void ProcessManager::update(float)
    {
        ProcessList::iterator i, e;

        if(mReportProcesses)
        {
            i = mRunningProcesses.begin();
            e = mRunningProcesses.end();
            for(; i != e; ++i)
            {               
                Process* process = *i;
                reportInfo("Active Processes", process->getReportName());
            }
        }

        if(!mDeadProcesses.empty())
        {
            i = mDeadProcesses.begin();
            e = mDeadProcesses.end();
            for(; i != e; ++i)
            {               
                Process* process = *i;
                SafeDelete(process);
            }
            mDeadProcesses.clear();
        }
    }

    void ProcessManager::addProcessToList(Process* process, ProcessList& processList)
    {
        ProcessList::iterator processIterator = std::find(processList.begin(), processList.end(), process);

        if(processIterator == processList.end())
        {
            processList.push_back(process);
        }
    }

    void ProcessManager::removeProcessFromList(Process* process, ProcessList& processList)
    {
        ProcessList::iterator processIterator = std::find(processList.begin(), processList.end(), process);

        if(processIterator != processList.end())
        {
            processList.erase(processIterator);
        }
    }

    void ProcessManager::addProcess(Process* process)
    {
        addProcessToList(process, mRunningProcesses);
    }

    void ProcessManager::removeProcess(Process* process)
    {
        removeProcessFromList(process, mRunningProcesses);

        if(process->mDeleteOnEnd)
        {
            addProcessToList(process, mDeadProcesses);
        }
    }

} // namespace core