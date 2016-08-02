// Copyright 2006-12 HumaNature Studios Inc.

#include "CorePch.h"

#include "Bind.h"

#include "BindServer.h"
#include "BoundTypeMember.h"
#include "Scripting/LuaScriptManager.h"

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

        // check the global context and update any matching values from the resultant properties
        boundMember->updateValue(LuaScriptManager::Instance().getGlobalContext()->getProperties());
    }

    void Bind::RemoveMember(const char* name)
    {
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

} // namespace core
