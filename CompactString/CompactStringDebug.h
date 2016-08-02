// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __COMPACTSTRINGDEBUG_H__
#define __COMPACTSTRINGDEBUG_H__

namespace core {

    extern void CompactStringHolderInit();
    extern void CompactStringHolderDeinit();
    extern void CompactStringHolderWriteToFile(const char* fileName);

    class CompactStringDebug:
        public CompactStringRelease
    {
    public:
        const char* mStringValue;

        CompactStringDebug();

        explicit CompactStringDebug(int crc);

        CompactStringDebug(const char* pString);

        CompactStringDebug(const std::string& s);

        CompactStringDebug(const CompactStringDebug& s);

        const char* c_str() const;

        CompactStringDebug& operator=(int crc);

        CompactStringDebug& operator=(const CompactStringDebug& s);

        using CompactStringRelease::operator==;
        using CompactStringRelease::operator<;
        using CompactStringRelease::operator!=;

        void clear();
    };

} // namespace core

#endif // __COMPACTSTRINGDEBUG_H__