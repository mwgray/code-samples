// Copyright 2006-13 HumaNature Studios Inc.

#ifndef __PLATFORMPRINT_H__
#define __PLATFORMPRINT_H__

namespace core {

    class PlatformPrint
    {
    public:
        static void initialize();

        static void finalize();
        
        static void print(const char* message, int priority);
    };

} // namespace core

#endif // __PLATFORMPRINT_H__