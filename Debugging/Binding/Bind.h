// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __BIND_H__
#define __BIND_H__

#include "BoundTypeMember.h"

namespace core {

    class BoundMember;
    class BindServer;
    struct BindServerConfig;

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
        void initialize(BindServerConfig* serverConfig);
        void finalize();

        void UpdateMembers(Property* property);

    protected:
        typedef std::vector<BoundMember*> BoundMemberVec;
        BoundMemberVec mMembers;
        bool mIsFinalized;

#ifdef __HNS_LIVEEDITSERVER_ENABLED__
        BindServer *mServer;
#endif // __HNS_LIVEEDITSERVER_ENABLED__

        // Type-specific implementations are in Bind.cpp.
        // Keeping the code out header lets us keep BindServer.h out of precompiled header.
        template<typename t_type>
        void AddToServer(BoundTypeMember<t_type>* typedMember);

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