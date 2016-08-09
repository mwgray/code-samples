//
// Created by Matthew Gray on 8/4/16.
//

#ifndef __SINGLETONMACROS_H__
#define __SINGLETONMACROS_H__

// Since Singleton's require some static members with some odd template syntax, these helper macros simplify the syntax

// request a semicolon at the end of a macro
#define SEMICOLON_REQUIRE_GLOBAL using namespace dontusethisnamespacedotcom

// defines CoreTypeName for a given type
#define DEFINE_TYPE(Namespace, Class) template <> const char* GetTypeName < Namespace::Class > () { return #Class; }

// singleton sInstance pointer
#define DEFINE_SINGLETON_DATA(Namespace, SingletonType) \
    template<> Namespace::SingletonType* core::Singleton < Namespace::SingletonType > :: sInstance = nullptr

// defines singleton sInstance pointer and CoreTypeName
#define DEFINE_SINGLETON_TYPE(Namespace, Class) DEFINE_TYPE(Namespace, Class); DEFINE_SINGLETON_DATA(Namespace, Class)

#endif //__SINGLETONMACROS_H__
