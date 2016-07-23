// Copyright 2006-13 HumaNature Studios Inc.

#include "PhyreCorePch.h"

#include "Humanature/Templates/Singleton/Singleton.h"

namespace core {

    std::map<std::string, SingletonHolderBase*>* SingletonHolderBase::sSingletonMap;

    

} // namespace core