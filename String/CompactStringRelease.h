// Copyright 2006-12 HumaNature Studios Inc.
#pragma once

namespace core {
    class CompactStringRelease
    {
    protected:
        const static int s_UninitializedCRC = 0xFFFFFFFF;
        int m_CRC;
        void Calc(const char* pString);

    public:
        CompactStringRelease() : m_CRC(s_UninitializedCRC) { }
        explicit CompactStringRelease(int crc) : m_CRC(crc) { }

        CompactStringRelease(const char* pString)
        {
            Calc(pString);
        }

        CompactStringRelease(const std::string& s)
        {
            Calc(s.c_str());
        }

        CompactStringRelease& operator=(const char* pString)
        {
            Calc(pString);
            return *this;
        }

        CompactStringRelease& operator=(int crc)
        {
            m_CRC = crc;
            return *this;
        }

        CompactStringRelease& operator=(const std::string& s)
        {
            Calc(s.c_str());
            return *this;
        }

        bool operator==(const CompactStringRelease& rhs) const
        {
            return m_CRC == rhs.m_CRC;
        }

        bool operator<(const CompactStringRelease& rhs) const
        {
            return m_CRC < rhs.m_CRC;
        }

        bool operator!=(const CompactStringRelease& rhs) const
        {
            return !(*this == rhs);
        }

        bool empty() const
        {
            return m_CRC == s_UninitializedCRC;
        }

        void clear()
        {
            m_CRC = s_UninitializedCRC;
        }

        int crc() const
        {
            return m_CRC;
        }
    };
} // namespace core
