// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __BIND_H__
#define __BIND_H__

#include "BoundTypeMember.h"

namespace core {

    class Bind
        : public Singleton<Bind>
    {
    public:

        // Main entry point of the system, Bind to a member variable
        template <typename t_type>
        static BoundMember& Member(t_type& value, void * host, const char* name)
        {
            BoundTypeMember<t_type>* typedMember = new BoundTypeMember<t_type>(value);
            typedMember->name(name);
            Instance().AddToServer(typedMember);
            Instance().AddMember(typedMember, host);
            return *typedMember;
        }

        // Main entry point of the system,  Bind to a global or static variable
        template <typename t_type>
        static BoundMember& StaticMember(t_type& value, const char* name)
        {
            return Member(value,nullptr,name);
        }

        Bind();
        ~Bind();
        void finalize();

        // Called after a lua script executes to update various members
        void UpdateMembers(Property* property);

    protected:
        typedef std::vector<BoundMember*> BoundMemberVec;
        BoundMemberVec mMembers;
        bool mIsFinalized;

        void AddMember(BoundMember* boundMember, void * host);

        void RemoveMember(const char* name);

        friend class Unbind;
    };

    class Unbind
    {
    public:
        // Main exit point of the system, removes a binding
        inline static void Member(const char* name)
        {
            Bind::Instance().RemoveMember(name);
        }
    };

} // namespace core

#endif // __BIND_H__