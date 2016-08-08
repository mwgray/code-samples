//
// Created by Matthew Gray on 8/4/16.
//

#ifndef __WORDCACHEPCH_H__
#define __WORDCACHEPCH_H__

#define assertThat(...)
#define logError(...)

#include <string>
#include <vector>

#include "Utf8.h"

typedef unsigned int U32;
typedef unsigned short U16;

namespace Phyre {

    typedef char PChar;
    typedef unsigned int PUInt32;
    typedef unsigned char PUInt8;
    typedef unsigned short PUInt16;

    template<typename T>
    class PArray
    {
    public:
        inline const T &operator [] (PUInt32 index) const
        {
            T t;
            return t;
        }
    };
}

#endif //__WORDCACHEPCH_H__
