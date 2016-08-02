// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __COMPACTSTRING_H__
#define __COMPACTSTRING_H__

#include "CompactStringRelease.h"
#include "CompactStringDebug.h"

namespace core {

#ifdef __HNS_KEEP_CRC_STRINGS__
    typedef CompactStringDebug CompactString;
#else // !__HNS_KEEP_CRC_STRINGS__
    typedef CompactStringRelease CompactString;
#endif // __HNS_KEEP_CRC_STRINGS__

} // namespace core

#endif // __COMPACTSTRING_H__