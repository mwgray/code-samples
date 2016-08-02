// Copyright 2006-12 HumaNature Studios Inc.

#include "PhyreCorePch.h"

#define Z_SOLO
#include <zlib.h>

#if defined(PHYRE_PLATFORM_WIN32)
    PHYRE_PRAGMA_COMMENT_LIB(zlib)
#elif defined(PHYRE_PLATFORM_PSP2)
    PHYRE_PRAGMA_COMMENT_LIB(z)
#elif defined(PHYRE_PLATFORM_PS3)
    PHYRE_PRAGMA_COMMENT_LIB(z)
#elif defined(PHYRE_PLATFORM_ORBIS)
    PHYRE_PRAGMA_COMMENT_LIB(zlib)
#endif

namespace core {

    StringVec& split(const std::string& s, char delim, StringVec& elems)
    {
        std::stringstream ss(s);
        std::string item;
        while(std::getline(ss, item, delim))
        {
            elems.push_back(item);
        }
        return elems;
    }

    StringVec split(const std::string& s, char delim)
    {
        StringVec elems;
        return split(s, delim, elems);
    }

    bool replace(std::string& str, const std::string& from, const std::string& to)
    {
        size_t start_pos = str.find(from);
        if(start_pos == std::string::npos)
            return false;
        str.replace(start_pos, from.length(), to);
        return replace(str, from, to);
    }

    char* InsertCommas(unsigned long value)
    {
        static  char    str[30];
        for (int ii = 0; ii < 30; ++ii) str[ii] = NULL;
        PHYRE_SNPRINTF(str, sizeof(str), "%u", (unsigned int)value);
        if (strlen(str) > 3) {
            memmove( &str[strlen(str)-3], &str[strlen(str)-4], 4 );
            str[strlen(str) - 4] = ',';
        }
        if (strlen(str) > 7) {
            memmove( &str[strlen(str)-7], &str[strlen(str)-8], 8 );
            str[strlen(str) - 8] = ',';
        }
        if (strlen(str) > 11) {
            memmove( &str[strlen(str)-11], &str[strlen(str)-12], 12 );
            str[strlen(str) - 12] = ',';
        }
        return str;
    }

    std::string join(const StringVec& strings, const char* joinString)
    {
        std::string joinedString;

        for(StringVec::const_iterator i = strings.begin(); i != strings.end(); ++i)
        {
            if(joinedString.size() > 0)
            {
                joinedString += joinString;
            }
            joinedString += *i;
        }

        return joinedString;
    }

    // compression

    bool compress(const char* inBuffer, int size, char* outBuffer, int& resultSize)
    {
        z_stream zlibStream;

        zlibStream.next_in = (Bytef*)const_cast<char*>(inBuffer);     /* next input byte */
        zlibStream.avail_in = size;  /* number of bytes available at next_in */
        zlibStream.total_in = 0;  /* total number of input bytes read so far */

        zlibStream.next_out = (Bytef*)outBuffer; /* next output byte should be put there */
        zlibStream.avail_out = resultSize; /* remaining free space at next_out */
        zlibStream.total_out = 0; /* total number of bytes output so far */

        zlibStream.msg = NULL;  /* last error message, NULL if no error */
        zlibStream.state = nullptr; /* not visible by applications */

        zlibStream.zalloc = nullptr;  /* used to allocate the internal state */
        zlibStream.zfree = nullptr;   /* used to free the internal state */
        zlibStream.opaque = nullptr;  /* private data object passed to zalloc and zfree */

        zlibStream.data_type = Z_TEXT;  /* best guess about the data type: binary or text */
        zlibStream.adler = 0;      /* adler32 value of the uncompressed data */
        zlibStream.reserved = 0;   /* reserved for future use */

        deflateInit2(&zlibStream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15 + 16, 8, Z_DEFAULT_STRATEGY);

        int res = deflate(&zlibStream, Z_FINISH);
        bool success = res == Z_STREAM_END;

        resultSize = zlibStream.total_out;

        res = deflateEnd(&zlibStream);

        return success && (res == Z_OK);
    }

    bool decompress(const char* inBuffer, int size, char* outBuffer, int& resultSize)
    {
        z_stream zlibStream;

        zlibStream.next_in = (Bytef*)const_cast<char*>(inBuffer);     /* next input byte */
        zlibStream.avail_in = size;  /* number of bytes available at next_in */
        zlibStream.total_in = 0;  /* total number of input bytes read so far */

        zlibStream.next_out = (Bytef*)outBuffer; /* next output byte should be put there */
        zlibStream.avail_out = resultSize - 1; /* remaining free space at next_out */
        zlibStream.total_out = 0; /* total number of bytes output so far */

        zlibStream.msg = NULL;  /* last error message, NULL if no error */
        zlibStream.state = nullptr; /* not visible by applications */

        zlibStream.zalloc = nullptr;  /* used to allocate the internal state */
        zlibStream.zfree = nullptr;   /* used to free the internal state */
        zlibStream.opaque = nullptr;  /* private data object passed to zalloc and zfree */

        zlibStream.data_type = Z_TEXT;  /* best guess about the data type: binary or text */
        zlibStream.adler = 0;      /* adler32 value of the uncompressed data */
        zlibStream.reserved = 0;   /* reserved for future use */

        inflateInit2(&zlibStream, 15 + 32);

        int res = inflate(&zlibStream, Z_FINISH);
        bool success = res == Z_STREAM_END;

        resultSize = zlibStream.total_out;
        outBuffer[resultSize] = 0;

        res = inflateEnd(&zlibStream);

        return success && (res == Z_OK);
    }

} // namespace core