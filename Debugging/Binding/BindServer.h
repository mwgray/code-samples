// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __BIND_SERVER_H__
#define __BIND_SERVER_H__

struct LiveServerConfig;
struct LiveServerConfigTyped;

#include <wws_liveedit/server.h>
#include <wws_liveedit/server_typed.h>

// author: mgraeb

namespace core {

    enum BindServerResult
    {
        BSR_SUCCESS,
        BSR_ERR_GENERIC,
        BSR_ERR_OUT_OF_NODES,
        BSR_ERR_OUT_OF_MEMORY,
        BSR_COUNT
    };

    // All configuration data for BindServer, LiveServerConfig, & LiveServerConfigTyped
    struct BindServerConfig
    {
        BindServerConfig();

        int mMaxNodes; // max number of items we can simultaneously show on the server
        int mMaxItemSize;
        int mLiveServerTypedMaxTypes; // max number of non-basic types to show. Must be non-zero or typed server won't work at all.
        const char* mRootName;
        int mMaxClientConnections;
        int mLiveServerUpdateTimeoutUs;
        int mTcpPort; // use -1 for LIVE_TCP_DEFAULT_PORT
    };

    // Extract LiveServerConfig & LiveServerConfigTyped from BindServerConfig
    void BindServerConfigToLiveServerConfigs(const BindServerConfig* bindConfig,
                                             ::LiveServerConfig* liveConfig,
                                             ::LiveServerConfigTyped* typedConfig);

    // Using LiveEdit server, expose variables bound via Bind::Member.
    // A remote LiveEdit client can connect to game for live viewing/editing of variables.
    class BindServer
    {
    public:
        BindServer(int maxNodes, const char* rootName);
        ~BindServer();

        template<typename t_type>
        void addMember(BoundTypeMember<t_type>* member);

        void removeMember(const BoundMember* val);

    protected:
        int mMaxNodes;
        int mNodeCount; // don't create more than mMaxNodes
        const char* mRootName; // name of top-level group under which all our nodes live
        LiveGroup mRootHandle; // handle to top-level group

        typedef std::map<const BoundMember*,LiveItem> BoundMemberNodeMap;
        BoundMemberNodeMap boundMemberNodes; // each BoundMember's LiveNode on server

        // Each unique t_type *must* have its own template specialization written
        template <typename t_type>
        LiveResult addTypedItem(t_type* val, const char* name, LiveItem* nodeOut);
    };

    BindServerResult bindServerPopulateLiveServerConfig(const BindServerConfig* bindConfig, LiveServerConfig* liveServerConfig, LiveServerConfigTyped* liveServerConfigTyped);
    BindServerResult bindServerSizeofInstance(const BindServerConfig* config, int* outSizeof);
    BindServerResult bindServerInitialize(const BindServerConfig* config, void* memBuffer, BindServer** outNewBindServer);
    BindServerResult bindServerShutdown(BindServer* bindServer);
};

template <typename t_type>
inline void core::BindServer::addMember(BoundTypeMember<t_type>* member)
{
    assertThat(member, isNotNull);
    assertThat(boundMemberNodes.find(member) == boundMemberNodes.end(), isTrue, "BindServer already tracking '%s'", member->getName());
    LiveItem node = 0x0;
    if (mNodeCount < mMaxNodes)
    {
        LiveResult result = addTypedItem(member->getDataPtr(), member->getName(), &node);
        if (result == LIVE_SUCCESS)
        {
            ++mNodeCount;
        }
        else
        {
            logFatal("Live Edit Server", "BindServer failed to add '%s'", member->getName());
        }
    }
    else
    {
        logFatal("Live Edit Server", "BindServer has used up all its nodes; not adding '%s'. Try increasing max nodes.", member->getName());
    }
    boundMemberNodes[member] = node;
}

#endif // __BIND_SERVER_H__
