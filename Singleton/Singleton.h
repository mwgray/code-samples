// Copyright 2006-13 HumaNature Studios Inc.

#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include "HumaNature/OS/ThreadDebugging.h"

#if defined(__SNC__)
#define HNS_FORCE_INLINE inline
#elif defined(_MSC_VER)
#define HNS_FORCE_INLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
#define HNS_FORCE_INLINE __attribute__((always_inline))
#else
#define HNS_FORCE_INLINE inline
#endif

namespace core {

    // for tracking all singletons at run-time
    class SingletonHolderBase
    {
    public:
        static std::map<std::string, SingletonHolderBase*>* sSingletonMap;
    };

    template<typename SingletonType>
    class Singleton
        : public SingletonHolderBase
    {
    protected:
        static SingletonType* sInstance;
    public:
        ///  Returns a reference to singleton object
        HNS_FORCE_INLINE static SingletonType& Instance()
        {
            return *sInstance;
        }

        HNS_FORCE_INLINE static SingletonType* InstancePtr()
        {
            return sInstance;
        }

        Singleton()
        {
            (*SingletonHolderBase::sSingletonMap)[GetCoreTypeName<SingletonType>()] = this;
        }

        virtual ~Singleton()
        {
            EXPECTMAINTHREAD;
            if (sInstance == this)
            {
                auto it = SingletonHolderBase::sSingletonMap->find(GetCoreTypeName<SingletonType>());
                SingletonHolderBase::sSingletonMap->erase(it);
                sInstance = nullptr;
            }
        }

        static SingletonType& Create()
        {
            if(sInstance == nullptr)
            {
                sInstance = new SingletonType;
            }
            return *sInstance;
        }

        static void Destroy()
        { 
            if (sInstance)
            {
                delete sInstance;
                sInstance = nullptr;
            }
        }

        static void UseInstance(SingletonType* inst)
        {
            sInstance = inst;
        }

    private:
        Singleton(const Singleton&);                 // Prevent copy-construction
        Singleton& operator=(const Singleton&);      // Prevent assignment
    };
    
} // namespace core

#endif // __SINGLETON_H__
