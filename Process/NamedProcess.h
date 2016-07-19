// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __NAMEDPROCESS_H__
#define __NAMEDPROCESS_H__

#include "Process.h"

namespace core {

    // TODO - rid ourselves of this when proper reflection is in
    class NamedProcess
        : public Process
    {
    public:
        NamedProcess(const char* name=nullptr);

    protected:
        std::string mName;

        const char* getReportName();        
    };

} // namespace core

#endif // __NAMEDPROCESS_H__