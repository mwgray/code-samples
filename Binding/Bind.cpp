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

    void Bind::finalize()
    {
        mIsFinalized = true;
        BindLifetime::Instance().finalize();
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

                delete boundMember;

                break;
            }
        }        
    }

    void Unbind::Host(void * host)
    {
        BindLifetime::Instance().DeregisterHost(host);
    }


} // namespace core
