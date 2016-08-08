// Copyright 2006-13 HumaNature Studios Inc.

#ifndef __WORDCACHE_H__
#define __WORDCACHE_H__

namespace core {

    // Collection of phrases, with lists to associate with each phrase.
    // Data is organized for fast lookup.
    class WordCache
    {
    protected:
        U32 mPhraseCount; // All arrays (besides the buffers) are the same length. This variable is unnecessary, but nice to have.

        // Buffer containing all phrases.
        // Each phrase ends with '\0'.
        // Text is stored as UTF-8.
        // Phrases are stored longest to shorted, phrases of same length are then sorted alphabetically.
        // Phrases should be lowercase.
        Phyre::PArray<Phyre::PChar> mPhraseBuffer;
        Phyre::PArray<Phyre::PUInt32> mPhrasesStart; // index, in buffer, of each phrase's first character
        Phyre::PArray<Phyre::PUInt8> mPhrasesLength; // length of each phrase in buffer (not counting '\0') (mgraebtodo: used anywhere?)

        // Buffer containing each phrase's lists of starred and unstarred dekos
        // All of a phrase's starred dekos are stored in a row, followed by that phrase's unstarred dekos.
        // The next deko in the buffer is the next phrase's first starred deko.
        Phyre::PArray<Phyre::PUInt16> mDekoBuffer;
        Phyre::PArray<Phyre::PUInt32> mDekosStart; // index, in buffer, of each phrase's first starred deko
        Phyre::PArray<Phyre::PUInt8> mDekosStarred; // number of starred dekos for each phrase
        Phyre::PArray<Phyre::PUInt8> mDekosUnstarred; // number of unstarred dekos for each phrase

    public:
        static const U32 kPhraseNotFound;

        struct Match
        {
            U32 phraseIndex; // phrase's index in wordcache
            U32 phraseInTextUnicodeIndex; // Number of UTF-8 characters preceding phrase in text (not the byte index)
        };

        // Search text for phrases. Fill outMatches with results.
        // text will be altered by this function.
        void findMatches(const char* text, std::vector<Match>& outMatches) const;

        // If phrase exists, returns its index. Else returns kPhraseNotFound
        inline U32 findPhrase(const char* str) const;

        inline U32 getPhraseCount() const;
        inline const char* getPhrase(U32 phraseI) const;

        inline U32 getStarredDekoCount(U32 phraseI) const;
        inline U16 getStarredDeko(U32 phraseI, U32 dekoI) const;

        inline U32 getUnstarredDekoCount(U32 phraseI) const;
        inline U16 getUnstarredDeko(U32 phraseI, U32 dekoI) const;

        // How to sort strA relative to strB:
        // <0: strA belongs before strB
        //  0: strA equals strB
        // >0: strA belongs after strB
        static inline int CompareStrings(const char* strA, size_t lenStrA, const char* strB, size_t lenStrB);

#ifdef PHYRE_TOOL_BUILD
        // Fill wordcache, based on jsonStr.
        // jsonStr is modified during read and cannot be used again.
        // returns true if no errors occurred.
        bool ReadJson(char* jsonStr);
#endif // PHYRE_TOOL_BUILD

    protected:
        inline U32 getStarredDekoIndex(U32 phraseI, U32 dekoI) const;
        inline U32 getUnstarredDekoIndex(U32 phraseI, U32 dekoI) const;
    };
}

#include "WordCache.inl"

#endif // __WORDCACHE_H__