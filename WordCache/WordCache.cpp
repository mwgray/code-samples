#include "WordCachePch.h"

#include "WordCache.h"

#ifdef PHYRE_TOOL_BUILD
#include <../../rapidjson/reader.h>
#endif // PHYRE_TOOL_BUILD

namespace core {
    using namespace Phyre;
//    PHYRE_BIND_START(WordCache)
//        PHYRE_ADD_MEMBER_FLAGS(DataMember, Phyre::PE_CLASS_DATA_MEMBER_READ_ONLY)
//            PHYRE_BIND_CLASS_DATA_MEMBER(mPhraseCount)
//            PHYRE_BIND_CLASS_DATA_MEMBER_PHYRE_ARRAY(mPhraseBuffer, PChar)
//            PHYRE_BIND_CLASS_DATA_MEMBER_PHYRE_ARRAY(mPhrasesStart, PUInt32)
//            PHYRE_BIND_CLASS_DATA_MEMBER_PHYRE_ARRAY(mPhrasesLength, PUInt8)
//            PHYRE_BIND_CLASS_DATA_MEMBER_PHYRE_ARRAY(mDekoBuffer, PUInt16)
//            PHYRE_BIND_CLASS_DATA_MEMBER_PHYRE_ARRAY(mDekosStart, PUInt32)
//            PHYRE_BIND_CLASS_DATA_MEMBER_PHYRE_ARRAY(mDekosStarred, PUInt8)
//            PHYRE_BIND_CLASS_DATA_MEMBER_PHYRE_ARRAY(mDekosUnstarred, PUInt8)
//    PHYRE_BIND_END

    const U32 WordCache::kPhraseNotFound = 0xFFFFFFFF;

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
        for (U32 phraseIdx = 0; phraseIdx < mPhraseCount; ++phraseIdx)
        {
            const char* phrase = getPhrase(phraseIdx);
            U32 phraseLen = mPhrasesLength[phraseIdx];

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

#ifdef PHYRE_TOOL_BUILD
    typedef rapidjson::UTF8<> WordCacheEncoding;

    struct WordCacheJsonHandler
    {
        typedef WordCacheEncoding::Ch Ch;

        struct Entry
        {
            std::string phrase;
            std::vector<int> starredDekos;
            std::vector<int> unstarredDekos;
        };
        typedef std::vector<Entry*> EntryVec;

        // For sorting Entries. Returns true if a goes before b.
        // Entries are sorted from from longest to shortest phrase,
        // phrases of same length are further sorted by memcmp.
        static bool CompareEntryPtrs(const Entry* a, const Entry* b)
        {
            return WordCache::CompareStrings(a->phrase.c_str(), a->phrase.size(), b->phrase.c_str(), b->phrase.size()) < 0;
        }

        Entry* mCurEntry;
        EntryVec mAllEntries;

        enum State
        {
            kReadyForBigList,
            kReadyForPhrase,
            kReadyForDekoLists,
            kReadyForStarredList,
            kReadyForStarredListValues,
            kReadyForUnstarredList,
            kReadyForUnstarredListValues,
            kReadyForDekoListsClose,
            kDone,
            kError,
        } mState;

        WordCacheJsonHandler()
        {
            mState = kReadyForBigList;
            mCurEntry = nullptr;
            mAllEntries.clear();
        }

        ~WordCacheJsonHandler()
        {
            delete mCurEntry;
            for (size_t i = 0; i < mAllEntries.size(); ++i)
            {
                delete mAllEntries[i];
            }
        }

        void SetError(const char* msg)
        {
            if (mState != kError)
            {
                PHYRE_PRINTF("WordCache JSON Error (state %d): %s\n", mState, msg);
                mState = kError;
            }
        }

        void StartObject()
        {
            switch (mState)
            {
            case kReadyForBigList:
                mState = kReadyForPhrase;
                break;
            default:
                SetError("unexpected {");
            }
        }

        void EndObject(rapidjson::SizeType /*memberCount*/)
        {
            switch (mState)
            {
            case kReadyForPhrase:
                mState = kDone;
                break;
            default:
                SetError("unexpected }");
            }
        }


        void StartArray()
        {
            switch (mState)
            {
            case kReadyForDekoLists:
                mState = kReadyForStarredList;
                break;
            case kReadyForStarredList:
                mState = kReadyForStarredListValues;
                break;
            case kReadyForUnstarredList:
                mState = kReadyForUnstarredListValues;
                break;
            default:
                SetError("unexpected [");
            }
        }

        void EndArray(rapidjson::SizeType /*elementCount*/)
        {
            switch (mState)
            {
            case kReadyForStarredListValues:
                mState = kReadyForUnstarredList;
                break;
            case kReadyForUnstarredListValues:
                mState = kReadyForDekoListsClose;
                break;
            case kReadyForDekoListsClose:
                mAllEntries.push_back(mCurEntry);
                mCurEntry = nullptr;
                mState = kReadyForPhrase;
                break;
            default:
                SetError("unexpected ]");
            }
        }

        void String(const Ch* str, rapidjson::SizeType /*length*/, bool /*copy*/)
        {
            if (mState == kReadyForPhrase)
            {
                mCurEntry = new Entry();
                mCurEntry->phrase = (char*)str;
                mState = kReadyForDekoLists;
            }
            else
            {
                SetError("unexpected string");
            }
        }

        void Int(int i)
        {
            switch (mState)
            {
            case kReadyForStarredListValues:
                mCurEntry->starredDekos.push_back(i);
                break;
            case kReadyForUnstarredListValues:
                mCurEntry->unstarredDekos.push_back(i);
                break;
            default:
                SetError("unexpected int");
            }
        }

        void Null() { PHYRE_ASSERT(false); }
        void Bool(bool /*b*/) { PHYRE_ASSERT(false); }
        void Uint(unsigned i) { Int((int)i); }
        void Int64(int64_t i) { Int((int)i); }
        void Uint64(uint64_t i) { Int((int)i); }
        void Double(double d) { Int((int)d); }
    };

    bool WordCache::ReadJson(char* jsonStr)
    {
        WordCacheJsonHandler handler;
        rapidjson::InsituStringStream strStream(jsonStr);
        rapidjson::GenericReader<WordCacheEncoding> reader(nullptr);
        reader.Parse<rapidjson::kParseInsituFlag>(strStream, handler);
        if (handler.mState != WordCacheJsonHandler::kDone)
        {
            PHYRE_PRINTF("WordCache JSON parsing failed\n");
            return false;
        }
        WordCacheJsonHandler::EntryVec& entries = handler.mAllEntries;

        // Ensure entries are sorted the way the game expects them to be sorted
        std::sort(entries.begin(), entries.end(), WordCacheJsonHandler::CompareEntryPtrs);

        // set lengths
        mPhraseCount = (U32)entries.size();
        mPhrasesStart.resize(mPhraseCount);
        mPhrasesLength.resize(mPhraseCount);
        mDekosStart.resize(mPhraseCount);
        mDekosStarred.resize(mPhraseCount);
        mDekosUnstarred.resize(mPhraseCount);

        // figure out total space needed for buffers
        U32 phraseBufferLength = 0;
        U32 dekoBufferLength = 0;
        for (U32 i = 0; i < mPhraseCount; ++i)
        {
            const WordCacheJsonHandler::Entry& entry = *entries[i];
            mPhrasesStart[i] = phraseBufferLength;

            const char* phrase = entry.phrase.c_str();
            U32 phraseLen = (U32)strlen(phrase);
            if (phraseLen > 0xFF)
            {
                PHYRE_PRINTF("WordCache phrase is too long: \"%s\"\n", phrase);
                return false;
            }

            mPhrasesLength[i] = (U8)phraseLen;
            phraseBufferLength += phraseLen + 1; // space for '\0' after each word

            if (entry.starredDekos.size() > 0xFF || entry.unstarredDekos.size() > 0xFF)
            {
                PHYRE_PRINTF("WordCache phrase has too many dekos: \"%s\"\n", phrase);
                return false;
            }

            mDekosStart[i] = dekoBufferLength;
            mDekosStarred[i] = (U8)entry.starredDekos.size();
            mDekosUnstarred[i] = (U8)entry.unstarredDekos.size();
            dekoBufferLength += mDekosStarred[i] + mDekosUnstarred[i];
        }

        // fill buffers
        mPhraseBuffer.resize(phraseBufferLength);
        mDekoBuffer.resize(dekoBufferLength);
        for (U32 i = 0; i < mPhraseCount; ++i)
        {
            const WordCacheJsonHandler::Entry& entry = *entries[i];
            const char* phrase = entry.phrase.c_str();

            // copy phrase into mPhraseBuffer
            memcpy(&mPhraseBuffer[mPhrasesStart[i]], phrase, mPhrasesLength[i] + 1);

            // copy starred dekos into mDekoBuffer
            for (U8 dekoI = 0; dekoI < entry.starredDekos.size(); ++dekoI)
            {
                int dekoVal = entry.starredDekos[dekoI];
                if (dekoVal < 0 || dekoVal > 0xFFFF)
                {
                    PHYRE_PRINTF("Wordcache has invalid deko value '%d' in phrase: \"%s\"\n", dekoVal, phrase);
                    return false;
                }
                mDekoBuffer[getStarredDekoIndex(i, dekoI)] = (U16)dekoVal;
            }

            // copy unstarred dekos into mDekoBuffer
            for (U8 dekoI = 0; dekoI < entry.unstarredDekos.size(); ++dekoI)
            {
                int dekoVal = entry.unstarredDekos[dekoI];
                if (dekoVal < 0 || dekoVal > 0xFFFF)
                {
                    PHYRE_PRINTF("Wordcache has invalid deko value '%d' in phrase: \"%s\"\n", dekoVal, phrase);
                    return false;
                }
                mDekoBuffer[getUnstarredDekoIndex(i, dekoI)] = (U16)dekoVal;
            }
        }
        return true;
    }
#endif // PHYRE_TOOL_BUILD

}