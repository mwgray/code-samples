// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __FNV1_H__
#define __FNV1_H__

namespace core {

    class FNV1
    {
    public:
        // if buffer size == 0, treat as null terminated buffer
        static unsigned int calculateHash(const void* buffer, I32 bufferSize = 0);
        static unsigned int calculateHashUnchanged(const void* buffer, I32 bufferSize = 0);

    protected:
        static void hash(U32& hashValue, U32 data);
    };

} // namespace core

#endif // __FNV1_H__