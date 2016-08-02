// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __BIND_H__
#define __BIND_H__

#include "BoundTypeMember.h"

namespace core {

    class BoundMember;

    class Bind
        : public Singleton<Bind>
    {
    public:

        template <typename t_type>
        static BoundMember& Member(t_type& value, void * host, const char* name)
        {
            BoundTypeMember<t_type>* typedMember = new BoundTypeMember<t_type>(value);
            typedMember->name(name);
            Instance().AddToServer(typedMember);
            Instance().AddMember(typedMember, host);
            return *typedMember;
        }

        template <typename t_type>
        static BoundMember& StaticMember(t_type& value, const char* name)
        {
            return Member(value,nullptr,name);
        }

        Bind();
        ~Bind();
        void finalize();

        void UpdateMembers(Property* property);

    protected:
        typedef std::vector<BoundMember*> BoundMemberVec;
        BoundMemberVec mMembers;
        bool mIsFinalized;

        void AddMember(BoundMember* boundMember, void * host);

        void RemoveMember(const char* name, bool handleLifetime = true);

        friend class Unbind;
        friend class BindLifetime;
    };

    class Unbind
    {
    public:
        inline static void Member(const char* name)
        {
            Bind::Instance().RemoveMember(name);
        }

        static void Host(void * host);
    };

} // namespace core

#endif // __BIND_H__