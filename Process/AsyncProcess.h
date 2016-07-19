// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __ASYNCPROCESS_H__
#define __ASYNCPROCESS_H__

#include <Utilities/NoCopy.h>
// #include "HumaNature/OS/ThreadDebugging.h"

namespace core {

    class BackgroundThread;

    class BackgroundJob : public NoCopy, public Castable
    {
        friend class BackgroundThread;
    protected:
        // Overload these in your derived class:

        // onReset occurs on the thread reset is called on (only for reusable jobs)
        virtual void onReset() {}
        // onStart occurs on main thread
        virtual void onStart() {}
        // Process occurs on BackgroundThread
        virtual void process() = 0;
        // OnComplete occurs on main thread
        virtual void onComplete() {};

    public:
        enum Status
        {
            Initializing,
            Waiting,
            Processed,
            Complete,
            Aborted
        };

        // Job adds itself to the given BackgroundThread when you call start.
        BackgroundJob(BackgroundThread& hostThread, bool reusable = false);
        BackgroundJob();
        virtual ~BackgroundJob();

        void setHostThread(BackgroundThread& hostThread);

        void reset();
        void start();
        void setAborted();

        Status getStatus() const
        {
            return mJobStatus;
        }

        const char * statusToString() const
        {
            switch (mJobStatus)
            {
            case Initializing: return "Initializing";
            case Waiting: return "Waiting";
            case Processed: return "Processed";
            case Complete: return "Complete";
            case Aborted: return "Aborted";
            }
            return "UnknownState";
        }

    protected:
        
        void doProcess();
        void doComplete();

        bool isReuseable() const
        {
            return mReusable;
        }
        
        // does not delete on job completion
        void setReuseable(bool reuseable)
        {
            mReusable = reuseable;
        }

        bool deleteWhenComplete() const
        { 
            return mDeleteWhenComplete; 
        }

        bool mDeleteWhenComplete;

    private:
        BackgroundThread* mHostThread;
        // volatile Status mStatus;

        Status mJobStatus;
        PCriticalSection* mStatusLock;
        bool mReusable;

        bool tryChangeStatus(Status from, Status to)
        {
            PCriticalSectionLock lock(*mStatusLock);
            if (mJobStatus == from)
            {
                mJobStatus = to;
                return true;
            }
            else
            {
                return false;
            }
        }

        void setStatus(Status to)
        {
            PCriticalSectionLock lock(*mStatusLock);
            mJobStatus = to;
        }
    };

    class BackgroundThread : public NoCopy, public Castable
    {
        friend BackgroundJob;

    public:
        BackgroundThread(const PChar * threadName, PUInt32 threadCount = 1);
        ~BackgroundThread();

        // 0 indicates to process all
        // returns the number of jobs processed
        PUInt32 updateMainThread(PUInt32 limitJobs = 0);
        // done on scene change
        void abortAll(bool wait);

        // wait until the thread has no active jobs
        void drain(bool update = false);
        void terminate();

        std::string const& getThreadName() const
        {
            return mThreadName;
        }

        PUInt32 getThreadCount() const
        {
            return mThreadCount;
        }

        PCurrentThreadID getThreadID(PUInt32 index) const
        {
            if (index < mThreadCount)
                return mThreads[index].mAsyncThreadID;
            else
                return 0;
        }

        bool getIsEmpty() const
        {
            return mIsEmpty;
        }

    private:
        void addJob(BackgroundJob * job);

        typedef std::vector<BackgroundJob*> BackgroundJobList;

        std::string mThreadName;
        BackgroundJobList mPendingJobs;
        BackgroundJobList mCompletedJobs;

        // mWaitEvent is used when mRequestOrder is empty, rather than spin-waiting on a new incoming request
        Phyre::PEventFlag mWaitEvent;
        // mRequestListLock is used when mAsyncRequests, mRequestOrder or mActiveRequest changes
        PCriticalSection mRequestListLock;
        // mStatusLock is used by BackgroundJob when changing status
        PCriticalSection mStatusLock;

        PAtomicType mThreadsTerminated;
        
        bool mTerminated;
        bool mIsEmpty;

        struct SingleThread
        {
            BackgroundThread * mHost;
            PUInt32 mIndex;
            Phyre::PThread mAsyncThread;
            PCurrentThreadID mAsyncThreadID;
            volatile bool mThreadActive;
            volatile BackgroundJob * mActiveJob;

            // mTerminateThreadLock governs where in the async process, it is safe to terminate that thread
            PCriticalSection mTerminateThreadLock;
        };

        PUInt32 mThreadCount;
        SingleThread* mThreads;
        Phyre::PCurrentThreadID mMainThread;

        static PUInt32 asyncThreadProcessFunction(void * userdata);
        void asyncThreadProcess(SingleThread& singleThread);
    };

#define EXPECTBGTHREAD(bgThread) ::core::ThreadDebugging::ConfirmExecutionThread( (bgThread).getThreadID(), __FUNCTION__ );

} // namespace core

#endif // __ASYNCPROCESS_H__