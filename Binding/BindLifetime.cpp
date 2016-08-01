// Copyright 2006-12 HumaNature Studios Inc.

#include "CorePch.h"

#include "BindLifetime.h"

DEFINE_SINGLETON_TYPE(core, BindLifetime);

namespace core {

    BindLifetime::BindLifetime()
    {
        mIsFinalized = false;
    }

    BindLifetime::~BindLifetime()
    {
        assertThat(mIsFinalized,isTrue,"BindLifetime being destroyed without being finalized");
    }

    void BindLifetime::finalize()
    {
        DeregisterHost(this);

        if (!mBindingToHostMap.empty())
        {
            // deregister these, but warn
            for (BindingToHostMap::iterator iter = mBindingToHostMap.begin(); iter != mBindingToHostMap.end(); ++iter)
            {
                std::string name = iter->first;
                logWarn("Binding","Binding [%s] was not removed!",name.c_str());
                Bind::Instance().RemoveMember(name.c_str(),false);
            }
        }

        mBindingToHostMap.clear();
        mBindingMap.clear();

        mIsFinalized = true;
    }

    void BindLifetime::RegisterBinding(void * host, std::string const& name)
    {
        assertThat(mBindingToHostMap.find(name) == mBindingToHostMap.end(),isTrue,"binding name \"%s\" is already in use.", name.c_str());

        if (host == nullptr)
            host = this;

        mBindingToHostMap[name] = host;
        mBindingMap[host].insert(name);
    }
    
    void BindLifetime::RegisterStaticBinding(std::string const& name)
    {
        // use "this" as host
        RegisterBinding(this, name);
    }
    
    void BindLifetime::UnregisterBinding(std::string const& name)
    {
        BindingToHostMap::iterator findHost = mBindingToHostMap.find(name);
        assertThat(findHost != mBindingToHostMap.end(),isTrue,"binding name \"%s\" is not known.",name.c_str());
        if (findHost == mBindingToHostMap.end())
            return;

        void * host = findHost->second;
        mBindingToHostMap.erase(findHost);

        mBindingMap[host].erase(name);
        if (mBindingMap[host].empty())
            mBindingMap.erase(host);
    }

    void BindLifetime::DeregisterHost(void * host)
    {
        BindingMap::iterator findHost = mBindingMap.find(host);
        if (findHost == mBindingMap.end())
            // nothing to do
            return;

        StringSet const& bindingNames = findHost->second;
        for (StringSet::const_iterator nameIter = bindingNames.begin(); nameIter != bindingNames.end(); ++nameIter)
        {
            std::string name = *nameIter;
            Bind::Instance().RemoveMember(name.c_str(),false);
            mBindingToHostMap.erase(name);
        }

        mBindingMap.erase(findHost);
    }

} // namespace core
