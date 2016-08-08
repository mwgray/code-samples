#include "WordCachePch.h"
#include "WordCache.h"

using namespace core;

class SampleWordCache
    : public WordCache {

public:
    SampleWordCache()
    {
        mPhrases.push_back("you guys");
        mPhrases.push_back("hello");
        mPhrases.push_back("cool");
        mPhrases.push_back("can");
        mPhrases.push_back("guy");  // is in the string, but shouldn't match because it is invalidated from the match 'you guys'
        mPhrases.push_back("his");  // is in the string, but shouldn't match because it is part of 'this'
        mPhrases.push_back("!");
        mPhrases.push_back("?");
    }
};

int main() {

    SampleWordCache wordCache;

    std::vector<WordCache::Match> matches;

    std::string s = "oh my, hello you guys!  can't you see how cool this is? cool!";

    wordCache.findMatches(s.c_str(), matches);

    printf("Match count: %u\n", matches.size());

    auto i = matches.begin();
    auto e = matches.end();

    for (; i < e; ++i) {
        auto match = *i;
        auto phrase = wordCache.getPhrase(match.phraseIndex);

        printf("Found phrase '%s' at index '%u'\n", phrase.c_str(), match.phraseInTextUnicodeIndex);
    }

    return 0;
}