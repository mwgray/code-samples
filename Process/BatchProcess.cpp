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
        , mNumAsyncProcessesRequested(0)
        , mNumAsyncProcessedCompleted(0)
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
            if (p->isAsnycProcess())
            {
                ++mNumAsyncProcessesRequested;
                p->addEventListener(ProcessEvent::PROCESS_ENDED, this, &BatchProcess::handleAsyncProcessEnded, true);
                p->begin();

            }
            else
            {
                mProcessList.push_back(p);
            }
        }
    }

    void BatchProcess::startNextProcess()
    {
        // process async list
        //ProcessList copyList(*this);
        //clear();
        //for (ProcessList::iterator iter = copyList.begin(); iter != copyList.end(); ++iter)
        //{
        //    Process * process = *iter;
        //    if (process->continueSupported())
        //    {
        //        process->begin();
        //        process->addEventListener(ProcessEvent::PROCESS_CONTINUE, this, &BatchProcess::handleProcessEnded, true);
        //    }
        //    else
        //        push_back(process);
        //}

        if(!mProcessList.empty())
        {  
            mCurrentProcess = *mProcessList.begin();
            mCurrentProcess->addEventListener(ProcessEvent::PROCESS_ENDED, this, &BatchProcess::handleProcessEnded, true);
            mCurrentProcess->begin();
        }
        else
        {
            checkDone();

        //    // Process::end();
        }

        // mProgress = float(mNumProcessesCompleted) / float(mNumProcessesCompleted + mNumAsyncProcessesRequested + mProcessList.size());
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

    void BatchProcess::handleAsyncProcessEnded(const Event*)
    {
        mNumProcessesCompleted++;
        mNumAsyncProcessedCompleted++;

        // mProgress = float(mNumProcessesCompleted) / float(mNumProcessesCompleted + mNumAsyncProcessesRequested + mProcessList.size());
        checkDone();
    }

    void BatchProcess::checkDone()
    {
        mProgress = float(mNumProcessesCompleted) / float(mNumProcessesCompleted + mNumAsyncProcessesRequested + mProcessList.size());

        if (mNumAsyncProcessedCompleted == mNumAsyncProcessesRequested && mProcessList.empty())
        {
            // is complete
            Process::end();
        }
    }

} // namespace core