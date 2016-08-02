// Copyright 2006-12 HumaNature Studios Inc.

#include "CorePch.h"

#include "BatchProcess.h"

#include "ProcessEvent.h"

DEFINE_TYPE_NS(core, BatchProcess);

namespace core {

    BatchProcess::BatchProcess() 
        : mProgress(0.0f)
        , mCurrentProcess(nullptr)
        , mNumProcessesCompleted(0)
    {
        mName = "BatchProcess";
        logInfo("BatchProcess","creating 0x%p",this);
    }

    BatchProcess::~BatchProcess()
    {
        logInfo("BatchProcess","deleting 0x%p",this);
    }


    void BatchProcess::onBegin()
    {
        startNextProcess();
    }

    void BatchProcess::AddProcess(Process* p)
    {
        if (NULL != p)
        {
            mProcessList.push_back(p);
        }
    }

    void BatchProcess::startNextProcess()
    {
        if(!mProcessList.empty())
        {  
            mCurrentProcess = *mProcessList.begin();
            mCurrentProcess->addEventListener(ProcessEvent::PROCESS_ENDED, this, &BatchProcess::handleProcessEnded, true);
            mCurrentProcess->begin();
        }
        else
        {
            checkDone();
        }
    }

    void BatchProcess::handleProcessEnded(const Event*)
    {
        mNumProcessesCompleted++;

        // pop the completed process from the list
        mProcessList.pop_front();
        mCurrentProcess = nullptr;

        startNextProcess();
        checkDone();
    }

    void BatchProcess::checkDone()
    {
        mProgress = float(mNumProcessesCompleted) / float(mProcessList.size());

        if (mProcessList.empty())
        {
            // is complete
            Process::end();
        }
    }

} // namespace core