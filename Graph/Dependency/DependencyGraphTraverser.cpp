// Copyright 2006-12 HumaNature Studios Inc.

#include "CorePch.h"

#include "DependencyGraphTraverser.h"

namespace core {
    const CompactString DependencyGraphTraverser::GRAPH_COMPLETE("graphComplete");

    void DependencyGraphTraverser::initialize(const DependencyGraphNode::List& dependencies, ICallback* callback)
    {
        mDependencies = dependencies;
        mCallback = callback;

        callback->mTraverser = this;
    }

    void DependencyGraphTraverser::traverse()
    {
        if(mDependencies.empty())
        {
            // empty list, complete immediately
            mProgress.mNumerator = mProgress.mDenominator = 1.0f;

            Event completeEvent(GRAPH_COMPLETE);
            dispatchEvent(completeEvent);
        }
        else
        {
            for (auto i = mDependencies.begin(); i != mDependencies.end(); ++i)
            {
                DependencyGraphNode* dependency = *i;

                if(isDependencyActive(dependency))
                {
                    continue;
                }
                else if(isDependencyCleared(dependency))
                {
                    continue;
                }
                else if(hasNoDependencies(dependency))
                {
                    mActiveDependencies.push_back(dependency);
                    mCallback->traverseNode(dependency);
                }
            }
        }
    }

    void DependencyGraphTraverser::clearDependecy(DependencyGraphNode* dependency)
    {
        // remove from active
        DependencyGraphNode::List::iterator dependencyIterator = std::find(mActiveDependencies.begin(), mActiveDependencies.end(), dependency);
        mActiveDependencies.erase(dependencyIterator);

        // add to cleared
        mClearedDependencies.push_back(dependency);

        mProgress.mNumerator = float(mClearedDependencies.size());
        mProgress.mDenominator = float(mDependencies.size());

        if(mProgress.mNumerator == mProgress.mDenominator)
        {
            Event completeEvent(GRAPH_COMPLETE);
            dispatchEvent(completeEvent);
        }
        else
        {
            // check for new dependencies
            traverse();
        }
    }

    bool DependencyGraphTraverser::hasNoDependencies(DependencyGraphNode* dependency)
    {
        for (auto i = dependency->mDependencies.begin(); i != dependency->mDependencies.end(); ++i)
        {
            DependencyGraphNode* dependency = *i;

            if(!isDependencyCleared(dependency))
            {
                return false;
            }
        }

        return true;
    }

    bool DependencyGraphTraverser::isDependencyActive(DependencyGraphNode* dependency)
    {
        return std::find(mActiveDependencies.begin(), mActiveDependencies.end(), dependency) != mActiveDependencies.end();
    }

    bool DependencyGraphTraverser::isDependencyCleared(DependencyGraphNode* dependency)
    {
        return std::find(mClearedDependencies.begin(), mClearedDependencies.end(), dependency) != mClearedDependencies.end();
    }
} // namespace core