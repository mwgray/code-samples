// Copyright 2006-12 HumaNature Studios Inc.

#include "CorePch.h"

#include "AsyncProcess.h"

DEFINE_TYPE_NS(core, BackgroundJob);
DEFINE_TYPE_NS(core, BackgroundThread);

namespace core {

    BackgroundJob::BackgroundJob(BackgroundThread& hostThread, bool reusable)
        : mDeleteWhenComplete(false)
        , mHostThread(&hostThread)
        , mJobStatus(Initializing)
        , mStatusLock(&hostThread.mStatusLock)
        , mReusable(reusable)
    {
    }

    BackgroundJob::BackgroundJob()
        : mDeleteWhenComplete(false)
        , mHostThread(nullptr)
        , mJobStatus(Initializing)
        , mStatusLock(nullptr)
        , mReusable(false)
    {
    }

    BackgroundJob::~BackgroundJob()
    {
    }

    void BackgroundJob::setHostThread(BackgroundThread& hostThread)
    {
        mHostThread = &hostThread;
        mStatusLock = &hostThread.mStatusLock;
    }

    void BackgroundJob::reset()
    {
        assertThat(mReusable, isTrue, "Cannot reset a non-resueable job");
        if (mJobStatus == Waiting || mJobStatus == Processed)
        {
            assertThatW(mJobStatus != Waiting && mJobStatus != Processed, isTrue, "Job should not be reset while in process");
            return;
        }

        mJobStatus = Initializing;
        onReset();
    }

    void BackgroundJob::start()
    {
        if (tryChangeStatus(Initializing,Waiting))
        {
            onStart();
            mHostThread->addJob(this);
        }
    }

    void BackgroundJob::setAborted()
    {
        setStatus(Aborted);
    }

    void BackgroundJob::doProcess()
    {
        MULTITHREAD_PROFILE_ZONE();

        if (getStatus() == Waiting)
        {
            process();
            tryChangeStatus(Waiting,Processed);
        }
    }

    void BackgroundJob::doComplete()
    {
        PROFILE_ZONE();

        EXPECT_THREAD(mHostThread->mMainThread);

        if (tryChangeStatus(Processed,Complete))
        {
            onComplete();
        }
    }

    ///////////////////////////////////////////

    BackgroundThread::BackgroundThread(const PChar * threadName, PUInt32 threadCount)
    {
        mMainThread = PhyreOS::GetCurrentThreadID();

        assertThat(1 <= threadCount && threadCount <= 8, isTrue, "Bad threadCount request %d, expects 1-8", threadCount);

        mThreadName = threadName;
        mTerminated = false;
        mIsEmpty = true;

        mRequestListLock.initialize();
        mWaitEvent.initialize();
        mStatusLock.initialize();

        mThreadCount = threadCount;
        mThreadsTerminated = 0;
        mThreads = new SingleThread [ threadCount ];

        for (PUInt32 i = 0; i < mThreadCount; ++i)
        {
            SingleThread& singleThread = mThreads[i];
            singleThread.mHost = this;
            singleThread.mIndex = i;
            singleThread.mTerminateThreadLock.initialize();
            // singleThread.mTerminateEvent.initialize();
            singleThread.mThreadActive = true;
            PThreadInitialization threadInit(&asyncThreadProcessFunction);
            threadInit.m_threadName = mThreadName.c_str();
            threadInit.m_userData = &singleThread;
            singleThread.mAsyncThread.initialize(threadInit);
        }
    }

    BackgroundThread::~BackgroundThread()
    {
        EXPECT_THREAD(mMainThread);

        safeAssertEquals(mTerminated,"BackgroundThread::terminate() has not been called");
    }

    void BackgroundThread::drain(bool update)
    {
        EXPECT_THREAD(mMainThread);

        // ensure all jobs are processed
        while (!mPendingJobs.empty())
        {
            PhyreOS::Sleep(5);
            updateMainThread();
        }

        // now, ensure all are complete
        for (PUInt32 i = 0; i < mThreadCount; ++i)
        {
            while (NULL != mThreads[i].mActiveJob)
                PhyreOS::Sleep(5);
        }

        if (update)
            updateMainThread();
    }


    void BackgroundThread::terminate()
    {
        EXPECT_THREAD(mMainThread);

        safeAssertEquals(!mTerminated,"BackgroundThread::terminate() has already been called");

        { //  PCriticalSectionSimpleLock lock(mTerminateThreadLock);
            for (PUInt32 i = 0; i < mThreadCount; ++i)
            {
                SingleThread& singleThread = mThreads[i];
                {
                    PCriticalSectionLock lock(singleThread.mTerminateThreadLock);
                    if (singleThread.mThreadActive)
                    {
                        singleThread.mThreadActive = false;
                        mWaitEvent.signal();
                        singleThread.mAsyncThread.terminate();
                    }
                }
            }
        } //  PCriticalSectionSimpleLock lock(mTerminateThreadLock);

        while (mThreadsTerminated < (PInt32)mThreadCount)
        {
            mWaitEvent.signal();
            PhyreOS::Sleep(0);
        }

        for (PUInt32 i = 0; i < mThreadCount; ++i)
        {
            SingleThread& singleThread = mThreads[i];
            singleThread.mTerminateThreadLock.terminate();
        }

        SafeDeleteArray(mThreads);

        mRequestListLock.terminate();
        mWaitEvent.terminate();
        mStatusLock.terminate();

        mTerminated = true;
    }


    PUInt32 BackgroundThread::updateMainThread(PUInt32 limitJobs)
    {
        PROFILE_ZONE();

        EXPECT_THREAD(mMainThread);

        size_t completedJobsCount = 0;
        BackgroundJob** completedJobs = nullptr;

        { // PCriticalSectionLock lock(mRequestListLock);
            PCriticalSectionLock lock(mRequestListLock);

            size_t totalCount = mCompletedJobs.size();
            if (totalCount)
            {
                completedJobs = (BackgroundJob**)alloca(sizeof(BackgroundJob*) * totalCount);

                while (!mCompletedJobs.empty() && ((completedJobsCount < limitJobs) || (limitJobs == 0)))
                {
                    completedJobs[completedJobsCount++] = mCompletedJobs.front();
                    mCompletedJobs.erase(mCompletedJobs.begin());
                }
            }
        }

        // issue the completed calls, and delete
        for (auto iter = completedJobs; iter != completedJobs + completedJobsCount; ++iter)
        {
            BackgroundJob* job = *iter;
            if (job->getStatus() == BackgroundJob::Processed)
            {
                job->doComplete();
            }

            if (!job->isReuseable())
            {
                delete job;
            }
        }

        return completedJobsCount;
    }

    // done on scene change
    void BackgroundThread::abortAll(bool wait)
    {
        PROFILE_ZONE();

        EXPECT_THREAD(mMainThread);

        BackgroundJobList pendingJobsToAbort;
        BackgroundJobList completedJobsToAbort;

        { // PCriticalSectionLock lock(mRequestListLock);
            PCriticalSectionLock lock(mRequestListLock);

            pendingJobsToAbort = mPendingJobs;
            mPendingJobs.clear();
            completedJobsToAbort = mCompletedJobs;
            mCompletedJobs.clear();

            for (PUInt32 i = 0; i < mThreadCount; ++i)
                if (NULL != mThreads[i].mActiveJob)
                {
                    BackgroundJob* activeJob = (BackgroundJob*)mThreads[i].mActiveJob;
                    activeJob->setAborted();
                }
        }

        for (BackgroundJobList::iterator iter = pendingJobsToAbort.begin(); iter != pendingJobsToAbort.end(); ++iter)
        {
            BackgroundJob * job = *iter;
            delete job;
        }

        for (BackgroundJobList::iterator iter = completedJobsToAbort.begin(); iter != completedJobsToAbort.end(); ++iter)
        {
            BackgroundJob * job = *iter;
            delete job;
        }

        if (wait)
        {
            for (PUInt32 i = 0; i < mThreadCount; ++i)
            {
                while (NULL != mThreads[i].mActiveJob)
                    PhyreOS::Sleep(5);
            }
        }
    }

    void BackgroundThread::addJob(BackgroundJob * job)
    {
        PROFILE_ZONE();

        if (NULL != job)
        { // PCriticalSectionLock lock(mRequestListLock);
            mIsEmpty = false;

            PCriticalSectionLock lock(mRequestListLock);

            mPendingJobs.push_back(job);
            if (!BuildPlatform::IsPS3)
                mWaitEvent.signal();

        } // PCriticalSectionLock lock(mRequestListLock);
    }

    PUInt32 BackgroundThread::asyncThreadProcessFunction(void * userdata)
    {
        SingleThread * singleThread = (SingleThread*)userdata;
        if (NULL != singleThread)
        {
            singleThread->mHost->asyncThreadProcess(*singleThread);
        }

        return 0;
    }

    void BackgroundThread::asyncThreadProcess(SingleThread& singleThread)
    {
        MULTITHREAD_PROFILE_ZONE();

        singleThread.mAsyncThreadID = PhyreOS::GetCurrentThreadID();
        ::core::ThreadDebugging::setThreadName(singleThread.mAsyncThreadID,mThreadName.c_str());

        singleThread.mActiveJob = NULL;

        while (singleThread.mThreadActive)
        { // while (singleThread.mThreadActive)
            singleThread.mTerminateThreadLock.lock();

            // locate next task
            { // PCriticalSectionLock lock(mRequestListLock);
                 
                PCriticalSectionLock lock(mRequestListLock);

                if (!mPendingJobs.empty())
                {
                    singleThread.mActiveJob = *mPendingJobs.begin();
                    mPendingJobs.erase(mPendingJobs.begin());
                }
                else
                {
                    // reset here, since this may signal before the wait below
                    if (!BuildPlatform::IsPS3)
                        mWaitEvent.reset();

                    mIsEmpty = true;
                }


            } // PCriticalSectionLock lock(mRequestListLock);

            if (NULL == singleThread.mActiveJob)
            {
                // pending jobs was empty
                singleThread.mTerminateThreadLock.unlock();
                if (!BuildPlatform::IsPS3)
                    mWaitEvent.wait();
                else
                    PhyreOS::Sleep(5);
            }
            else
            {
                // volatile -> non volatile
                BackgroundJob* activeJob = (BackgroundJob*)singleThread.mActiveJob;
                activeJob->doProcess();

                { // PCriticalSectionLock lock(mRequestListLock);
                    PCriticalSectionLock lock(mRequestListLock);
                    if (activeJob->getStatus() == BackgroundJob::Aborted)
                    {
                        delete activeJob;
                    }
                    else if (activeJob->deleteWhenComplete())
                    {
                        delete activeJob;
                    }
                    else
                    {
                        mCompletedJobs.push_back(activeJob);
                    }
                } // PCriticalSectionLock lock(mRequestListLock);

                singleThread.mActiveJob = NULL;

                singleThread.mTerminateThreadLock.unlock();
            }

        } // while (singleThread.mThreadActive)

        AtomicIncrement( &mThreadsTerminated, 1 );
//        singleThread.mTerminateEvent.signal();
    }

} // namespace core