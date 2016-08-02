// Copyright 2006-12 HumaNature Studios Inc.

#include "PhyreCorePch.h"

#include "HumaNature/Math/Crypto/FNV1.h"

namespace core {

    U32 FNV1::calculateHash(const void* buffer, I32 bufferSize)
    {
        // Implementation of FNV-1 Hash. if bufferSize == 0, Assumes null terminated strings
        static const U32 kFNVOffsetBasis = 2166136261;

        U32 hashValue = kFNVOffsetBasis;
        const U8* charBuffer = (const U8*)buffer;

        if(bufferSize == 0)
        {
            const U8* charIter = charBuffer;

            while(*charIter != '\0')
            {
                if(*charIter == '\\')
                {
                    hash(hashValue, (U32)'/');
                }
                else
                {
                    hash(hashValue, (U32)tolower(*charIter));
                }

                charIter++;
            }
        }
        else
        {            
            for(I32 i = 0; i < bufferSize; ++i)
            {
                hash(hashValue, (U32)(charBuffer[i]));
            }
        }
        return hashValue;
    }

    U32 FNV1::calculateHashUnchanged(const void* buffer, I32 bufferSize)
    {
        // Implementation of FNV-1 Hash. if bufferSize == 0, Assumes null terminated strings
        static const U32 kFNVOffsetBasis = 2166136261;

        U32 hashValue = kFNVOffsetBasis;
        const U8* charBuffer = (const U8*)buffer;

        if(bufferSize == 0)
        {
            const U8* charIter = charBuffer;

            while(*charIter != '\0')
            {
                hash(hashValue, (U32)*charIter);
                charIter++;
            }
        }
        else
        {            
            for(I32 i = 0; i < bufferSize; ++i)
            {
                hash(hashValue, (U32)(charBuffer[i]));
            }
        }
        return hashValue;
    }

    void FNV1::hash(U32& hashValue, U32 data)
    {
        static const U32 kFNVPrime = 16777619;

        hashValue *= kFNVPrime;
        hashValue ^= data;
    }

} // namespace core