// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __BATCHPROCESS_H__
#define __BATCHPROCESS_H__

#include "NamedProcess.h"
#include "ProcessTypes.h"

namespace core {

    class BatchProcess
        : public NamedProcess
        , public NoCopy
        // , public ProcessList
    {
    public:
        BatchProcess();
        ~BatchProcess();

        inline float getProgress()
        {
            return mProgress;
        }
        
        // note, will auto-run any async-enabled process
        void AddProcess(Process* p);

    protected:
        float mProgress;
        Process* mCurrentProcess;
        int mNumProcessesCompleted;
        int mNumAsyncProcessesRequested;
        int mNumAsyncProcessedCompleted;
        ProcessList mProcessList;

        virtual void onBegin();

        virtual void startNextProcess();

        void handleProcessEnded(const Event* event);
        void handleAsyncProcessEnded(const Event* event);
        void checkDone();
    };

} // namespace core

#endif // __BATCHPROCESS_H__