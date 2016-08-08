#include "WordCachePch.h"

#include "WordCache.h"

namespace core {

    // implementation taken from WordCacheDataLookupImpl.as
    void WordCache::findMatches(const char* originalText, std::vector<Match>& matches) const
    {
        matches.clear();
        size_t textLen = strlen(originalText);

        // make local modifiable copy to work with
        char* text = new char[textLen + 1];
        if (!text)
        {
            logError("WordCache", "findMatches failed to make local copy");
            return;
        }
        memcpy(text, originalText, textLen + 1);
        const char* textEnd = text + textLen;

        // cache unicode-index at start of each unicode character
        U32* textUnicodeIdx = new U32[textLen];
        if (!textUnicodeIdx)
        {
            assertThat(textUnicodeIdx, isNotNull);
            delete[] text;
            return;
        }
        U32 unichrCount = 0;
        for (const char* unichr = text; unichr < textEnd; unichr = Utf8::NextChar(unichr))
        {
            textUnicodeIdx[unichr - text] = unichrCount++;
        }

        // go through all the words in the word cache
        for (U32 phraseIdx = 0; phraseIdx < mPhrases.size(); ++phraseIdx)
        {
            const std::string& phraseString = getPhrase(phraseIdx);
            const char* phrase = phraseString.c_str();
            U32 phraseLen = phraseString.length();

            // for each instance of phrase in the text
            const char* phraseInText = Utf8::StrStr(text, textLen, phrase, phraseLen);
            while (phraseInText)
            {
                // look up adjacent characters
                const char* nextCharAddr = phraseInText + phraseLen;
                U32 nextChar = Utf8::Decode(nextCharAddr);

                const char* prevCharAddr;
                U32 prevChar;
                if (phraseInText == text)
                {
                    prevCharAddr = 0;
                    prevChar = 0;
                }
                else
                {
                    prevCharAddr = Utf8::PrevChar(phraseInText);
                    prevChar = Utf8::Decode(prevCharAddr);
                }

                // accept or reject match based on further criteria:
                // TODO: what about non-english languages?
                bool isMatch;
                // 1) Always accept phrases of single-character-punctuation.
                //      - ex: "!"
                if (*Utf8::NextChar(phrase) == 0 &&
                    iswpunct((wint_t)Utf8::Decode(phrase)))
                {
                    isMatch = true;
                }
                // 2) Reject phrase that occurs within a larger word.
                //      - ex: reject "hen" when it's part of "then"
                else if (iswalpha((wint_t)prevChar) ||
                         iswalpha((wint_t)nextChar))
                {
                    isMatch = false;
                }
                // 3) Reject contractions.
                //      - ex: reject "can" when it's part of "can't"
                else if (prevChar == '\'' &&
                         prevCharAddr != text &&
                         iswalpha((wint_t)Utf8::Decode(Utf8::PrevChar(prevCharAddr)))) // check left
                {
                    isMatch = false;
                }
                else if (nextChar == '\'' &&
                         nextCharAddr != textEnd &&
                         iswalpha((wint_t)Utf8::Decode(Utf8::NextChar(nextCharAddr)))) // check right
                {
                    isMatch = false;
                }
                // 4) Otherwise, accept
                else
                {
                    isMatch = true;
                }

                // handle match, increment and search for next instance of phrase
                if (isMatch)
                {
                    // replace phrase in text with *'s so it isn't flagged for further matches
                    memset(const_cast<char*>(phraseInText), '*', phraseLen);

                    // find unicode index of phrase in original string
                    U32 phraseInTextUnicodeIdx = textUnicodeIdx[phraseInText - text];
                    const Match m = {phraseIdx, phraseInTextUnicodeIdx};
                    matches.push_back(m);

                    phraseInText = nextCharAddr;
                }
                else
                {
                    phraseInText = Utf8::NextChar(phraseInText);
                }

                phraseInText = Utf8::StrStr(phraseInText, textEnd - phraseInText, phrase, phraseLen);
            }
        }

        delete[] text;
        delete[] textUnicodeIdx;
    }
}