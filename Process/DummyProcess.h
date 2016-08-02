// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __DUMMYPROCESS_H__
#define __DUMMYPROCESS_H__

#include "Process.h"

namespace core {

    // Dummy class if you need to group or create empty processes
    class DummyProcess
        : public Process
    {
    public:
        DummyProcess(const char* name);

    protected:
        virtual void onBegin();

        virtual void onEnd();
    };

} // namespace core

#endif // __DUMMYPROCESS_H__