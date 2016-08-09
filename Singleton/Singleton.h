// Copyright 2006-13 HumaNature Studios Inc.

#ifndef __SINGLETON_H__
#define __SINGLETON_H__

namespace core {

    // for tracking all singletons at run-time
    class SingletonBase
    {
    public:
        static std::map<std::string, SingletonBase*>* sSingletonMap;
    };

    template<typename SingletonType>
    class Singleton
        : public SingletonBase
    {
    protected:
        static SingletonType* sInstance;

    public:
        //  Returns a reference to singleton object
        static SingletonType& Instance()
        {
            return *sInstance;
        }

        //  Returns a pointer to singleton object
        static SingletonType* InstancePtr()
        {
            return sInstance;
        }

        Singleton()
        {
            // add to global list
            (*SingletonBase::sSingletonMap)[GetTypeName<SingletonType>()] = this;
        }

        virtual ~Singleton()
        {
            if (sInstance == this)
            {
                // remove from global list
                auto it = SingletonBase::sSingletonMap->find(GetTypeName<SingletonType>());
                SingletonBase::sSingletonMap->erase(it);
                sInstance = nullptr;
            }
        }

        // explicitly create
        static SingletonType& Create()
        {
            if(sInstance == nullptr)
            {
                sInstance = new SingletonType;
            }
            return *sInstance;
        }

        // explicitly destroy
        static void Destroy()
        {
            if (sInstance)
            {
                delete sInstance;
                sInstance = nullptr;
            }
        }
        
        // use to override a singleton with a new one
        // note- does not dispose of old one
        static void Override(SingletonType* inst)
        {
            sInstance = inst;
        }

    private:
        // Prevent copy-construction
        Singleton(const Singleton&);                 
        
        // Prevent assignment
        Singleton& operator=(const Singleton&);      
    };
    
} // namespace core

#endif // __SINGLETON_H__
