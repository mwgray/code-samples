// Copyright 2006-13 HumaNature Studios Inc.

#ifndef __WORDCACHE_H__
#define __WORDCACHE_H__

namespace core {

    // Collection of phrases, with lists to associate with each phrase.
    // Data is organized for fast lookup.
    class WordCache
    {
    protected:
        std::vector<std::string> mPhrases;

    public:
        struct Match
        {
            unsigned int phraseIndex; // phrase's index in word cache
            unsigned int phraseInTextUnicodeIndex; // Number of UTF-8 characters preceding phrase in text (not the byte index)
        };

        inline const std::string& getPhrase(unsigned int index) const
        {
            return mPhrases[index];
        }

        // Search text for phrases. Fill outMatches with results.
        // text will be altered by this function.
        void findMatches(const char* text, std::vector<Match>& outMatches) const;
    };
}

#endif // __WORDCACHE_H__