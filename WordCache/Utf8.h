// Copyright 2006-13 HumaNature Studios Inc.

#ifndef __UTF8_H__
#define __UTF8_H__

namespace core {

    // Helper functions for UTF-8.
    // http://en.wikipedia.org/wiki/UTF-8
    class Utf8
    {
    public :
        // Given Unicode codepoint, writes UTF-8 sequence to buffer.
        // Does not automatically null-terminate buffer.
        // Returns address after the last byte written.
        //
        // Ex: Given 0x20AC (Euro symbol),
        //     3 bytes are written to buffer (0xE2 0x82 0xAC),
        //     buffer+3 is returned
        //
        // Note: Encoded character could take up to 4 bytes in buffer
        static inline char* Encode(unsigned int codepoint, char* dstBuffer);

        // Like Encode, but won't write beyond end of buffer and always null-terminates buffer.
        // Note: Encoded character, plus null-terminator, could take up to 5 bytes in buffer
        static inline char* SafeEncode(unsigned int codepoint, char* dstBuffer, size_t dstBufferLen);

        // Given Unicode codepoint, returns null-terminated UTF-8 sequence in a std::string.
        static inline std::string EncodeToStdStr(unsigned int codepoint);

        // Given Unicode codepoint, returns number of bytes in UTF-8 encoding
        static inline unsigned int EncodedLen(unsigned int codepoint);

        // Given UTF-8 encoded string, returns address of next encoded character
        static inline const char* NextChar(const char* buffer);
        static inline char* NextChar(char* buffer); 

        // Given UTF-8 encoded string, returns address of previous encoded character
        static inline const char* PrevChar(const char* buffer);
        static inline char* PrevChar(char* buffer);

        // Count number of UTF-8 encoded characters until null-terminator or bufferEnd is reached
        static inline size_t CharCount(const char* buffer, const char* bufferEnd);
        // Like CharCount, but counts null-terminator if it precedes bufferEnd
        static inline size_t CharCountIgnoreNull(const char* buffer, const char* bufferEnd);

        // Returns codepoint encoded at given address.
        // If outNextChar specified, it's set to address of next UTF-8 character.
        static inline unsigned int Decode(const char* buffer, const char** outNextChar=nullptr);

        // Returns address of first occurrence of subStr in searchStr.
        // Returns null if subStr not found in searchStr.
        // searchStrLen and subStrLen are number of bytes until null terminator.
        static inline const char* StrStr(const char* searchStr, size_t searchStrLen, const char* subStr, size_t subStrLen);
        static inline const char* StrStr(const char* searchStr, const char* subStr);
    };
} // namespace core

#include "Utf8.inl"

#endif // __UTF8_H__