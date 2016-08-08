// Copyright 2006-13 HumaNature Studios Inc.

#ifndef __UTF8_INL__
#define __UTF8_INL__

namespace core {

    char* Utf8::Encode(unsigned int codepoint, char* dstBuffer)
    {
        if (codepoint <= 0x7F)
        {
            *dstBuffer++ = codepoint & 0xFF;
        }
        else if (codepoint <= 0x7FF)
        {
            *dstBuffer++ = 0xC0 | ((codepoint >> 6) & 0xFF);
            *dstBuffer++ = 0x80 | ((codepoint & 0x3F));
        }
        else if (codepoint <= 0xFFFF)
        {
            *dstBuffer++ = 0xE0 | ((codepoint >> 12) & 0xFF);
            *dstBuffer++ = 0x80 | ((codepoint >> 6) & 0x3F);
            *dstBuffer++ = 0x80 | (codepoint & 0x3F);
        }
        else
        {
            assertThat(codepoint, isLessThanEqualTo(0x10FFFF), "Invalid Unicode codepoint '0x%08X'", codepoint);
            *dstBuffer++ = 0xF0 | ((codepoint >> 18) & 0xFF);
            *dstBuffer++ = 0x80 | ((codepoint >> 12) & 0x3F);
            *dstBuffer++ = 0x80 | ((codepoint >> 6) & 0x3F);
            *dstBuffer++ = 0x80 | (codepoint & 0x3F);
        }
        return dstBuffer;
    }

    char* Utf8::SafeEncode(unsigned int codepoint, char* dstBuffer, size_t dstBufferLen)
    {
        assertThat(dstBuffer, isNotNull);
        assertThat(dstBufferLen, isGreaterThan((size_t)0));
        if (dstBuffer && dstBufferLen > 0)
        {
            if (EncodedLen(codepoint) < dstBufferLen)
            {
                dstBuffer = Encode(codepoint, dstBuffer);
            }
            *dstBuffer = 0;
        }
        return dstBuffer;
    }

    std::string Utf8::EncodeToStdStr(unsigned int codepoint)
    {
        char buffer[5];
        SafeEncode(codepoint, buffer, sizeof(buffer));
        return buffer;
    }

    unsigned int Utf8::EncodedLen(unsigned int codepoint)
    {
        if (codepoint <= 0x7F)          return 1;
        else if (codepoint <= 0x7FF)    return 2;
        else if (codepoint <= 0xFFFF)   return 3;
        else                            return 4;
    }

    const char* Utf8::NextChar(const char* buffer)
    {
        // run forwards until a byte does not start with 10xxxxxx
        while ((*++buffer & 0xC0) == 0x80) {}
        return buffer;
    }

    char* Utf8::NextChar(char* buffer)
    {
        return const_cast<char*>(NextChar(buffer));
    }

    const char* Utf8::PrevChar(const char* buffer)
    {
        // run backwards until a byte does not start with 10xxxxxx
        while ((*--buffer & 0xC0) == 0x80) {}
        return buffer;
    }

    size_t Utf8::CharCount(const char* buffer, const char* bufferEnd)
    {
        size_t count = 0;
        while (buffer < bufferEnd && *buffer != 0)
        {
            ++count;
            buffer = NextChar(buffer);
        }
        return count;
    }

    size_t Utf8::CharCountIgnoreNull(const char* buffer, const char* bufferEnd)
    {
        size_t count = 0;
        while (buffer < bufferEnd)
        {
            ++count;
            buffer = NextChar(buffer);
        }
        return count;
    }

    const char* Utf8::StrStr(const char* searchStr, size_t searchStrLen, const char* subStr, size_t subStrLen)
    {
    	// note: regular strstr might also work fine	
        const char* lastPossibleSearch = searchStr + (searchStrLen - subStrLen);
        while (searchStr <= lastPossibleSearch)
        {
            if (memcmp(searchStr, subStr, subStrLen) == 0)
            {
                return searchStr;
            }

            searchStr = NextChar(searchStr);
        }

        return nullptr;
    }

    unsigned int Utf8::Decode(const char* buffer, const char** outNextChar)
    {
        unsigned int codepoint;
        const unsigned char c = *buffer;
        if (c <= 0x7F) // 0x7F=01111111
        {
            // 1 byte
            codepoint = *buffer++;
        }
        else if (c <= 0xDF) // 0xDF=11011111
        {
            // 2 bytes
            assertThat(c, isGreaterThanEqualTo(0xC0));
            codepoint = ((*buffer++ & 0x1F) << 6) | // 00011111             // 0x1F << 6
                         (*buffer++ & 0x3F);        //       00111111       // 0x3F << 0
        }
        else if (c <= 0xEF) // 0xEF=11101111
        {
            // 3 bytes
            assertThat(c, isGreaterThanEqualTo(0xE0));
            codepoint = ((*buffer++ & 0x0F) << 12) | // 00001111             // 0x0F << 12
                        ((*buffer++ & 0x3F) <<  6) | //       00111111       // 0x3F <<  6
                         (*buffer++ & 0x3F);         //             00111111 // 0x3F <<  0
        }
        else
        {
            // 4 bytes
            assertThat(c >= 0xF0 && c <= 0xF7, isTrue);
            codepoint = ((*buffer++ & 0x07) << 18) | // 00000111                   // 0x7f << 18
                        ((*buffer++ & 0x3F) << 12) | //       00111111             // 0x3F << 12
                        ((*buffer++ & 0x3F) <<  6) | //             00111111       // 0x3F <<  6
                         (*buffer++ & 0x3F);         //                   00111111 // 0x3F <<  0
        }

        if (outNextChar)
        {
            *outNextChar = buffer;
        }

        return codepoint;
    }

    const char* Utf8::StrStr(const char* searchStr, const char* subStr)
    {
        return StrStr(searchStr, strlen(searchStr), subStr, strlen(subStr));
    }
}

#endif // __UTF8_INL__