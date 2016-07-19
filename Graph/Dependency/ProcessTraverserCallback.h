// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __PROCESSTRAVERSERCALLBACK_H__
#define __PROCESSTRAVERSERCALLBACK_H__

#include "DependencyGraphTraverser.h"
#include "Process/DummyProcess.h"
#include "Process/ProcessEvent.h"

namespace core {

    class ProcessTraverserCallback
        : public DependencyGraphTraverser::ICallback
    {
    public:
        virtual ~ProcessTraverserCallback();

        void registerProcess(const CompactString& processName, Process* process);

    protected:
        typedef std::map<CompactString, Process*> StringProcessMap;
        typedef std::map<const Process*, CompactString> ProcessStringMap;
        typedef std::map<const Process*, DependencyGraphNode*> NodeMap;

        StringProcessMap mStringProcessMap;
        ProcessStringMap mProcessStringMap;
        NodeMap mNodeMap;

        virtual void traverseNode(DependencyGraphNode* node);

        void handleProcessEnded(const Event* event);
    };

} // namespace core

#endif // __PROCESSTRAVERSERCALLBACK_H__