// Copyright 2006-12 HumaNature Studios Inc.

#include "CorePch.h"

#include "BindServer.h"

namespace core {

    BindServerConfig::BindServerConfig()
        : mMaxNodes(128)
        , mMaxItemSize(1024)
        , mLiveServerTypedMaxTypes(1)
        , mRootName("Bind")
        , mMaxClientConnections(2)
        , mLiveServerUpdateTimeoutUs(1000)
        , mTcpPort(-1)
    {}

    void BindServerConfigToLiveServerConfigs(const BindServerConfig* bindConfig,
                                             ::LiveServerConfig* liveConfig,
                                             ::LiveServerConfigTyped* typedConfig)
    {
        assertThat(bindConfig, isNotNull);
        assertThat(liveConfig, isNotNull);
        assertThat(typedConfig, isNotNull);

        liveConfig->maxConnections = bindConfig->mMaxClientConnections;
        liveConfig->maxNodeNameLength = 64;
        liveConfig->connectionTypes = LIVE_CONNECTION_TYPE_TCP;
#if WWS_TARGET_OS_PSP2
        liveConfig->connectionTypes = LIVE_CONNECTION_TYPE_DECI4P;
#elif WWS_TARGET_OS_PS3
        liveConfig->connectionTypes = LIVE_CONNECTION_TYPE_DECI3;
#endif
//        liveConfig->metadataAndShadowPoolSize = bindConfig->mLiveServerTypedMaxTypes * 1024; // magic number ... not sure why
        liveConfig->metadataAndShadowPoolSize = bindConfig->mMaxNodes * bindConfig->mMaxItemSize; // magic number ... not sure why
        liveConfig->tcp.port = (bindConfig->mTcpPort < 0) ? LIVE_TCP_DEFAULT_PORT : (uint16_t)bindConfig->mTcpPort;
        liveConfig->maxNodes = bindConfig->mMaxNodes;
        liveConfig->maxItemSize = bindConfig->mMaxItemSize;

        typedConfig->maxTypes = bindConfig->mLiveServerTypedMaxTypes;
    }

    BindServer::BindServer(int maxNodes, const char* rootName)
        : mMaxNodes(maxNodes)
        , mNodeCount(0)
        , mRootName(rootName)
        , mRootHandle(0x0)
    {
        LiveResult result;
        result = liveServerCreateGroup(0, mRootName, &mRootHandle);
        if (result == LIVE_SUCCESS)
        {
            ++mNodeCount;
        }
        else
        {
            logFatal("Live Edit Server", "BindServer can't create root node '%s'", mRootName);
        }
    }

    BindServer::~BindServer()
    {
        LiveResult result;
        if (mRootHandle != 0x0)
        {
            result = liveServerDestroyNode(mRootHandle);
            assertThat(result, isEqualTo(LIVE_SUCCESS), "BindServer failed to remove its root node '%s'", mRootName);
        }
    }

    template<>
    LiveResult BindServer::addTypedItem(int* val, const char* name, LiveItem* nodeOut)
    {
        return liveServerCreateInt32Item(mRootHandle, name, val, INT_MIN, INT_MAX, nodeOut);
    }

    template<>
    LiveResult BindServer::addTypedItem(float* val, const char* name, LiveItem* nodeOut)
    {
        return liveServerCreateFloatItem(mRootHandle, name, val, FLT_MIN, FLT_MAX, nodeOut);
    }

    template<>
    LiveResult BindServer::addTypedItem(bool* val, const char* name, LiveItem* nodeOut)
    {
        // compiler should optimize out all but one
        if (sizeof(bool) == 1)
            return liveServerCreateUInt8Item(mRootHandle, name, (U8*)val, 0, 1, nodeOut);
        else if(sizeof(bool) == 2)
            return liveServerCreateUInt16Item(mRootHandle, name, (U16*)val, 0, 1, nodeOut);
        else if(sizeof(bool) == 4)
            return liveServerCreateUInt32Item(mRootHandle, name, (U32*)val, 0, 1, nodeOut);
        else
        {
            logFatal("Live Edit Server", "bool has unexpected size");
            *nodeOut = 0x0;
            return -1;
        }
    }

    void BindServer::removeMember(const BoundMember* val)
    {
        assertThat(val, isNotNull);
        LiveResult result;

        // remove entry from boundMemberNodes
        BoundMemberNodeMap::iterator entry = boundMemberNodes.find(val);
        if (entry == boundMemberNodes.end())
        {
            logFatal("Live Edit Server", "BindServer has no entry for BoundMember '%s'", val->getName());
            return;
        }
        LiveNode node = entry->second;
        boundMemberNodes.erase(entry);

        // remove node from LiveEditServer
        if (node != 0x0)
        {
            result = liveServerDestroyNode(node);
            if (result == LIVE_SUCCESS)
            {
                --mNodeCount;
            }
            else
            {
                logFatal("Live Edit Server", "BindServer failed to remove node 0x%08x for BoundMember '%s'", node, val->getName());
            }
        }
    }

} // namespace core