// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __DUMMYPROCESS_H__
#define __DUMMYPROCESS_H__

#include "NamedProcess.h"

namespace core {

    class DummyProcess
        : public NamedProcess
    {
    public:
        DummyProcess(const char* name);

    protected:
        virtual void onBegin();

        virtual void onEnd();
    };

} // namespace core

#endif // __DUMMYPROCESS_H__