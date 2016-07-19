// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __DEPENDENCYGRAPHTRAVERSER_H__
#define __DEPENDENCYGRAPHTRAVERSER_H__

#include "Event/EventDispatcher.h"
#include "Math/Percentage.h"
#include "DependencyGraphNode.h"

namespace core {

    class DependencyGraphTraverser
        : public EventDispatcher
    {
    public:
        static const CompactString GRAPH_COMPLETE;

        class ICallback
        {
        protected:
            DependencyGraphTraverser* mTraverser;

            virtual ~ICallback() {};

            virtual void traverseNode(DependencyGraphNode* node) = 0;

            friend DependencyGraphTraverser;
        };

        void initialize(const DependencyGraphNode::List& dependencies, ICallback* callback);

        void traverse();

        void clearDependecy(DependencyGraphNode* dependency);

    protected:
        DependencyGraphNode::List mDependencies;
        DependencyGraphNode::List mActiveDependencies;
        DependencyGraphNode::List mClearedDependencies;

        Percentage mProgress;
        ICallback* mCallback;

        // TODO - optimize these later if needed
        bool hasNoDependencies(DependencyGraphNode* dependency);
        bool isDependencyActive(DependencyGraphNode* dependency);
        bool isDependencyCleared(DependencyGraphNode* dependency);
    };

} // namespace core

#endif // __DEPENDENCYGRAPHTRAVERSER_H__