//
// Created by Matthew Gray on 8/4/16.
//

#ifndef __SINGLETONPCH_H__
#define __SINGLETONPCH_H__

#include <map>
#include <string>

namespace core {

    // A global function that maps a given type to a string.  Use partial template specialization to define it
    // The DEFINE_TYPE macro will do this for you.
    template<typename T>
    const char *GetTypeName();
} // namespace core

#include "SingletonMacros.h"
#include "Singleton.h"

#endif //__SINGLETONPCH_H__
