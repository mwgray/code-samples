// Copyright 2006-12 HumaNature Studios Inc.

#include "CorePch.h"

#include "Process.h"
#include "ProcessEvent.h"
#include "ProcessManager.h"
#include "Scaleform/ExternalInterfaceEvent.h"

namespace core {

    Process::Process(const char* name)
        : mName(name == nullptr ? "" : name)
        , mBegun(false)
        , mEnded(false)
        , mDeleteOnEnd(true)
    {

    }

    Process::~Process()
    {

    }

    bool Process::hasBegun()
    {
        return mBegun;
    }

    bool Process::hasEnded()
    {
        return mEnded;
    }

    void Process::begin()
    {
        mBegun = true;

        ProcessManager::Instance().addProcess(this);

        logInfo("Process", "Begun process '%s'", getReportName());

        dispatch(ProcessEvent::PROCESS_BEGAN);

        onBegin();
    }

    void Process::end(const char* additionalEvent)
    {
        onEnd();

        mEnded = true;

        ProcessManager::Instance().removeProcess(this);

        logInfo("Process", "Ended process '%s'", getReportName());

        if(additionalEvent != nullptr)
        {
            dispatch(additionalEvent);
        }

        DelayedEventDispatcher::Instance().delayDispatchNonScaleform(this,ProcessEvent::PROCESS_ENDED);
        // dispatch(ProcessEvent::PROCESS_ENDED);
    }

    void Process::reset()
    {
        mBegun = false;
        mEnded = false;
    }

    const char* Process::getReportName()
    {
        return mName.c_str();
    }

    void Process::dispatch(const CompactString& type)
    {
        ProcessEvent event(type, *this);

        dispatchEvent(event);
    }

    void Process::onBegin()
    {
        // do nothing
    }

    void Process::onEnd()
    {
        // do nothing
    }

 } // namespace core

