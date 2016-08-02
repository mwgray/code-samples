// Copyright 2006-12 HumaNature Studios Inc.

#include "PhyreCorePch.h"

#include "FNV1.h"

namespace core {
    void CompactStringRelease::Calc(const char* pString)
    {
        SCOPED_TRACKED_PHASE_EX(CompactString);

        if(pString && *pString)
        {
            m_CRC = static_cast<int>(FNV1::calculateHash(pString));
            assertThat(m_CRC, isNotEqualTo(0xFFFFFFFF), "Uh oh! Calculated CRC is the uninitialized value, might want to try a different CompactString!");
        }
        else
        {
            m_CRC = s_UninitializedCRC;
        }
    }
} // namespace core