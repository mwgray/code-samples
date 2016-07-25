// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __BINDLIFETIME_H__
#define __BINDLIFETIME_H__

#include "Bind.h"

namespace core {

  class BindLifetime : public Singleton<BindLifetime>
  {
  public:
      BindLifetime();
      ~BindLifetime();

      void RegisterBinding(void * host, std::string const& name);
      void RegisterStaticBinding(std::string const& name);

      void UnregisterBinding(std::string const& name);

      void DeregisterHost(void * host);

      void finalize();

  private:
      typedef std::set<std::string> StringSet;
      typedef std::map<void*,StringSet> BindingMap;
      typedef std::map< std::string, void*> BindingToHostMap;

      BindingMap mBindingMap;
      BindingToHostMap mBindingToHostMap;
      bool mIsFinalized;
  };

} // namespace core

#endif // __BIND_H__