// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __DEPENDENCYGRAPHNODE_H__
#define __DEPENDENCYGRAPHNODE_H__

namespace core {

    class DependencyGraphNode
        : public MemoryObject
    {
    public:
        typedef std::list<DependencyGraphNode*> List;

        const CompactString mId;

        DependencyGraphNode(const CompactString& id);

        inline void addDependency(DependencyGraphNode* node)
        {
            mDependencies.push_back(node);
        }

    protected:
        List mDependencies;

        friend class DependencyGraphTraverser;
        friend class LuaDependencyGraphWriter;
        
        DependencyGraphNode& operator=(const DependencyGraphNode& rhs);
    };

} // namespace core

#endif // __DEPENDENCYGRAPHNODE_H__