// Copyright 2006-12 HumaNature Studios Inc.

#include "CorePch.h"

#include "Bind.h"

#include "BindServer.h"
#include "BoundTypeMember.h"
#include "Scripting/LuaScriptManager.h"

#include "BindLifetime.h"

DEFINE_SINGLETON_TYPE(core, Bind);

namespace core {

    Bind::Bind()
#ifdef __HNS_LIVEEDITSERVER_ENABLED__
        : mServer(nullptr)
#endif // __HNS_LIVEEDITSERVER_ENABLED__
    {
        mIsFinalized = false;
    }

    Bind::~Bind()
    {
        safeAssertEquals(mIsFinalized);
        for(BoundMemberVec::iterator i = mMembers.begin(); i != mMembers.end(); ++i)
        {
            SafeDelete(*i);
        }
    }

    void Bind::initialize(BindServerConfig* serverConfig)
    {
#ifdef __HNS_LIVEEDITSERVER_ENABLED__
        mServer = new BindServer(serverConfig->mMaxNodes, serverConfig->mRootName);
#else
        UNUSED_PARAM(serverConfig);
#endif // __HNS_LIVEEDITSERVER_ENABLED__
    }

    void Bind::finalize()
    {
        mIsFinalized = true;
        BindLifetime::Instance().finalize();

#ifdef __HNS_LIVEEDITSERVER_ENABLED__
        if (mServer)
        {
            for(BoundMemberVec::iterator i = mMembers.begin(); i != mMembers.end(); ++i)
            {
                mServer->removeMember(*i);
            }
        }
        SafeDelete(mServer);
#endif // __HNS_LIVEEDITSERVER_ENABLED__
    }

    void Bind::UpdateMembers(Property* property)
    {
        for(BoundMemberVec::iterator i = mMembers.begin(); i != mMembers.end(); ++i)
        {
            BoundMember* boundMember = *i;
            boundMember->updateValue(property);
        }
    }

    void Bind::AddMember(BoundMember* boundMember, void* host)
    {
        mMembers.push_back(boundMember);

        boundMember->updateValue(LuaScriptManager::Instance().getGlobalContext()->getProperties());

        BindLifetime::Instance().RegisterBinding(host,boundMember->getName());
    }

    void Bind::RemoveMember(const char* name, bool handleLifetime)
    {
        if (handleLifetime)
        {
            BindLifetime::Instance().UnregisterBinding(name);
        }

        for(BoundMemberVec::iterator i = mMembers.begin(); i != mMembers.end(); ++i)
        {
            BoundMember* boundMember = *i;
            if(!strcmp(boundMember->getName(), name))
            {         
                mMembers.erase(i);

#ifdef __HNS_LIVEEDITSERVER_ENABLED__
                if(mServer)
                {
                    mServer->removeMember(boundMember);
                }
#endif // __HNS_LIVEEDITSERVER_ENABLED__

                delete boundMember;

                break;
            }
        }        
    }


    // Putting template specialization here in cpp file, rather than implementing one function in .h file,
    // so that I don't have to include BindServer.h in Bind.h (which is in the precompiled header).
    // Is this more work and less graceful? Possibly.
    // mgraeb
#ifdef __HNS_LIVEEDITSERVER_ENABLED__
    #define ADD_TO_SERVER_TYPED(t_type)                              \
        template<>                                                   \
        void Bind::AddToServer(BoundTypeMember<t_type>* typedMember) \
        {                                                            \
            if (mServer)                                             \
                mServer->addMember(typedMember);                     \
        }
#else
    #define ADD_TO_SERVER_TYPED(t_type)                              \
        template<>                                                   \
        void Bind::AddToServer(BoundTypeMember<t_type>*)             \
        {                                                            \
        }

#endif // __HNS_LIVEEDITSERVER_ENABLED__

    ADD_TO_SERVER_TYPED(int)
    ADD_TO_SERVER_TYPED(bool)
    ADD_TO_SERVER_TYPED(float)

    void Unbind::Host(void * host)
    {
        BindLifetime::Instance().DeregisterHost(host);
    }


} // namespace core
