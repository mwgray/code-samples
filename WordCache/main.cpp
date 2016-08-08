#include "WordCachePch.h"
#include "WordCache.h"

using namespace core;

class SampleWordCache
    : public WordCache {

public:
    SampleWordCache()
    {
        mPhrases.push_back("Hello");
        mPhrases.push_back("you guys");
        mPhrases.push_back("can");
        mPhrases.push_back("cool");
        mPhrases.push_back("guy");
    }
};

int main() {

    SampleWordCache wordCache;

    std::vector<WordCache::Match> matches;

    printf("Checking matches\n");

    std::string s = "Oh my, Hello you guys!  Can't you see how cool this is?";

    wordCache.findMatches(s.c_str(), matches);
    printf("Finding matches in string '%s'\n", s.c_str());

    printf("Match count: %u\n", matches.size());

    std::vector<WordCache::Match>::iterator i = matches.begin();
    std::vector<WordCache::Match>::iterator e = matches.end();

    for (; i < e; ++i) {
        auto match = *i;
        auto phrase = wordCache.getPhrase(match.phraseIndex);

        printf("Match phrase '%s' at index '%u'\n", phrase.c_str(), match.phraseInTextUnicodeIndex);
    }

    return 0;
}