// Copyright 2006-12 HumaNature Studios Inc.

#include "CorePch.h"

#include "NamedProcess.h"

namespace core {

    NamedProcess::NamedProcess(const char* name)
        : mName(name == nullptr ? "" : name)
    {

    }

    const char* NamedProcess::getReportName()
    {
        return mName.c_str();
    }

} // namespace core