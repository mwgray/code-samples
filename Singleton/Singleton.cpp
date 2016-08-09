// Copyright 2006-13 HumaNature Studios Inc.

#include "SingletonPch.h"

namespace core {

    std::map<std::string, SingletonBase*>* SingletonBase::sSingletonMap = new std::map<std::string, SingletonBase*>();

} // namespace core