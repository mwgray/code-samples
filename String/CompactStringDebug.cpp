// Copyright 2006-12 HumaNature Studios Inc.

#include "PhyreCorePch.h"

#include "HumaNature/Templates/Singleton/Singleton.h"
#include "Humanature/Math/Crypto/FNV1.h"

#define KEEP_ORIGINAL_STRING_CASE

#if defined(__HNS_WIN32__) && defined(__HNS_KEEP_CRC_STRINGS__) && (defined(__HNS_DEBUG__) || defined(__HNS_DEBUGOPT__))
#define ENABLE_CRC_VALIDATE
#endif

static void cleanupString(std::string& str)
{
#ifndef KEEP_ORIGINAL_STRING_CASE
    for (auto it = str.begin(); it != str.end(); ++it)
    {
        if (*it == '\\')
        {
            *it = '/';
        }
        else
        {
            *it = (char)tolower(*it);
        }
    }
#else
    UNUSED_PARAM(str);
#endif
}

namespace core {
    class CompactStringHolder : public Singleton<CompactStringHolder>
    {
    public:
        Phyre::PCriticalSection mMapCS;
        std::map<int, std::string> mStringDB;

        CompactStringHolder()
        {
            mMapCS.initialize();
        }

        virtual ~CompactStringHolder()
        {
            mMapCS.terminate();
        }

        const char* getString(int crc, const char* str, bool& create)
        {
#ifdef KEEP_ORIGINAL_STRING_CASE
            // hack: ignore crc coming in, recompute.
            crc = FNV1::calculateHashUnchanged(str, 0);
#endif

            Phyre::PCriticalSectionLock lock(mMapCS);

            auto entry = mStringDB.find(crc);

            if (entry != mStringDB.end())
            {
                create = false;
                return entry->second.c_str();
            }

            if (!create)
            {
                return nullptr;
            }

            std::string& newStr = mStringDB[crc];
            newStr = str;

            cleanupString(newStr);

            // We keep these string around until shutdown, so this pointer will stay valid
            // cppcheck-suppress stlcstr
            return newStr.c_str();
        }

        // active in windows builds only:
        void writeTableToFile(FILE* fp)
        {
            UNUSED_PARAM(fp);
#ifdef __HNS_WIN32__
            if (NULL == fp)
                return;

            fprintf(fp,"%d entries\n\n", (int)mStringDB.size());

            for (auto iter = mStringDB.begin(); iter != mStringDB.end(); ++iter)
            {
                fprintf(fp,"0x%8x,\t\"%s\",\n", (unsigned int)iter->first, iter->second.c_str());
            }
#endif
        }
    };

    void CompactStringHolderInit()
    {
        CompactStringHolder::Create();
    }

    void CompactStringHolderDeinit()
    {
        CompactStringHolder::Destroy();
    }

    void CompactStringHolderWriteToFile(const char* fileName)
    {
        UNUSED_PARAM(fileName);
#ifdef __HNS_WIN32__
        FILE* stringTableFile;
        if (0 == fopen_s(&stringTableFile, fileName, "wt"))
        {
            CompactStringHolder::Instance().writeTableToFile(stringTableFile);
            fclose(stringTableFile);
        }
#endif
    }
}

DEFINE_SINGLETON_TYPE(core, CompactStringHolder);

namespace core {

    static inline void validate(const char* inputStr, const char* resultStr)
    {
        UNUSED_PARAM(inputStr);
        UNUSED_PARAM(resultStr);
#ifdef ENABLE_CRC_VALIDATE
        if (_stricmp(resultStr, inputStr))
        {
            // clean up string
            std::string lowerStr = inputStr;
            cleanupString(lowerStr);
            assertThat(resultStr, isEqualTo(lowerStr), "CRC collision! Change one of: '%s' or '%s'", inputStr, resultStr);
        }
#endif
    }

    CompactStringDebug::CompactStringDebug()
        : CompactStringRelease()
        , mStringValue(nullptr)
    {
    }

    CompactStringDebug::CompactStringDebug(int crc)
        : CompactStringRelease(crc)
    {
        bool created = false;
        mStringValue = CompactStringHolder::Instance().getString(crc, nullptr, created);
    }

    CompactStringDebug::CompactStringDebug(const char* pString)
        : CompactStringRelease(pString)
    {
        pString = pString ? pString : "";
        bool created = true;
        mStringValue = CompactStringHolder::Instance().getString(m_CRC, pString, created);
        if (!created)
        {
            validate(pString, mStringValue);
        }
    }

    CompactStringDebug::CompactStringDebug(const std::string& s)
        : CompactStringRelease(s)
    {
        bool created = true;
        mStringValue = CompactStringHolder::Instance().getString(m_CRC, s.c_str(), created);
        if (!created)
        {
            validate(s.c_str(), mStringValue);
        }
    }

    CompactStringDebug::CompactStringDebug(const CompactStringDebug& s)
        : CompactStringRelease(s.m_CRC)
    {
        mStringValue = s.mStringValue;
    }

    const char* CompactStringDebug::c_str() const
    {
        if(mStringValue && (*mStringValue != 0))
        {
            return mStringValue;
        }
        else
        {
            return "";
        }
    }

    CompactStringDebug& CompactStringDebug::operator=(int crc)
    {
        CompactStringRelease::operator=(crc);
        bool created = false;
        mStringValue = CompactStringHolder::Instance().getString(crc, nullptr, created);

        return *this;
    }

    CompactStringDebug& CompactStringDebug::operator=(const CompactStringDebug& s)
    {
        CompactStringRelease::operator=(s);
        mStringValue = s.mStringValue;

        return *this;
    }

    void CompactStringDebug::clear()
    {
        CompactStringRelease::clear();

        mStringValue = nullptr;
    }

    template <> std::string ToString(const CompactStringDebug& val)
    {
        return val.c_str();
    }

} // namespace core