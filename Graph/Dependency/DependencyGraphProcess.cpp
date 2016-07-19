// Copyright 2006-12 HumaNature Studios Inc.

#include "CorePch.h"

#include "DependencyGraphProcess.h"

DEFINE_TYPE_NS(core, DependencyGraphProcess);

namespace core {

    void DependencyGraphProcess::initialize(const DependencyGraphNode::List& nodes)
    {
        mTraverser.initialize(nodes, &mCallback);
    }

    void DependencyGraphProcess::onBegin()
    {
        mTraverser.addEventListener(DependencyGraphTraverser::GRAPH_COMPLETE, this, &DependencyGraphProcess::handleGraphComplete);

        mTraverser.traverse();
    }

    void DependencyGraphProcess::handleGraphComplete(const Event*)
    {
        mTraverser.removeEventListener(DependencyGraphTraverser::GRAPH_COMPLETE, this, &DependencyGraphProcess::handleGraphComplete);

        end();
    }

} // namespace core