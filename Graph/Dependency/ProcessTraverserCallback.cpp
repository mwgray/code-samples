// Copyright 2006-12 HumaNature Studios Inc.

#include "CorePch.h"

#include "ProcessTraverserCallback.h"

DEFINE_TYPE_NS(core, ProcessTraverserCallback);

namespace core {
    ProcessTraverserCallback::~ProcessTraverserCallback()
    {
        for(auto i = mNodeMap.begin(); i != mNodeMap.end(); ++i)
        {
            const Process* process = i->first;
            SafeDelete(process);
        }

        mNodeMap.clear();
        mStringProcessMap.clear();
        mProcessStringMap.clear();
    }

    void ProcessTraverserCallback::registerProcess(const CompactString& processName, Process* process)
    {
        mStringProcessMap[processName] = process;
        mProcessStringMap[process] = processName;
    }

    void ProcessTraverserCallback::traverseNode(DependencyGraphNode* node)
    {
        CompactString processId = node->mId;

        StringProcessMap::iterator processIterator = mStringProcessMap.find(processId);

        Process* process;

        if(processIterator != mStringProcessMap.end())
        {
            process = processIterator->second;
        }
        else
        {
            logWarn("Dependecy Graph", "Unknown node '%s'.  Defaulting to DummyProcess", processId.c_str());
            process = HNS_NEW(DummyProcess)(processId.c_str());
            registerProcess(processId, process);
        }

        logInfo("ProcessTraverserCallback", "Started Dependency Process '%s'", processId.c_str());

        mNodeMap[process] = node;
        process->addEventListener(ProcessEvent::PROCESS_ENDED, this, &ProcessTraverserCallback::handleProcessEnded, true);
        process->begin();
    }

    void ProcessTraverserCallback::handleProcessEnded(const Event* event)
    {
        const ProcessEvent* processEvent = static_cast<const ProcessEvent*>(event);

        const Process* process = &processEvent->mProcess;

        NodeMap::const_iterator iter = mNodeMap.find(process);
        assertThatW(iter != mNodeMap.end(), isTrue, "Can't find process!");
        DependencyGraphNode* node = iter->second;

        const char* processId = mProcessStringMap[process].c_str();
        logInfo("ProcessTraverserCallback", "Cleared Dependency Process '%s'", processId);

        mTraverser->clearDependecy(node);
    }
} // namespace core