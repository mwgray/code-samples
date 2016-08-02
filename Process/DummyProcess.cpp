// Copyright 2006-12 HumaNature Studios Inc.

#include "CorePch.h"

#include "DummyProcess.h"

namespace core {

    DummyProcess::DummyProcess(const char* name)
        : Process(name)
    {
        mName = "Dummy::" + mName;
    }

    void DummyProcess::onBegin()
    {
        end();
    }

    void DummyProcess::onEnd()
    {

    }

} // namespace core