// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __DEPENDENCYGRAPHPROCESS_H__
#define __DEPENDENCYGRAPHPROCESS_H__

#include "Process/Process.h"

#include "DependencyGraphNode.h"
#include "ProcessTraverserCallback.h"
#include "DependencyGraphTraverser.h"

namespace core {

    class DependencyGraphProcess
        : public Process
    {
    public:
        void initialize(const DependencyGraphNode::List& nodes);

        virtual void onBegin();

        void handleGraphComplete(const Event*);    

    protected:
        DependencyGraphTraverser mTraverser;
        ProcessTraverserCallback mCallback;
    };

} // namespace core

#endif // __DEPENDENCYGRAPHPROCESS_H__