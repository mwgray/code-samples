// Copyright 2006-13 HumaNature Studios Inc.

#ifndef __WORDCACHE_INL__
#define __WORDCACHE_INL__

namespace core {

//    U32 WordCache::getPhraseCount() const
//    {
//        return mPhraseCount;
//    }
//
    const char* WordCache::getPhrase(U32 phraseI) const
    {
//        assertThatE(phraseI, isLessThan(mPhraseCount));
        return &mPhraseBuffer[mPhrasesStart[phraseI]];
    }
//
//    U32 WordCache::getStarredDekoCount(U32 phraseI) const
//    {
//        assertThatE(phraseI, isLessThan(mPhraseCount));
//        return mDekosStarred[phraseI];
//    }
//
//    U16 WordCache::getStarredDeko(U32 phraseI, U32 dekoI) const
//    {
//        return mDekoBuffer[getStarredDekoIndex(phraseI, dekoI)];
//    }
//
//    U32 WordCache::getUnstarredDekoCount(U32 phraseI) const
//    {
//        assertThatE(phraseI, isLessThan(mPhraseCount));
//        return mDekosUnstarred[phraseI];
//    }
//
//    U16 WordCache::getUnstarredDeko(U32 phraseI, U32 dekoI) const
//    {
//        return mDekoBuffer[getUnstarredDekoIndex(phraseI, dekoI)];
//    }
//
//    int WordCache::CompareStrings(const char* strA, size_t lenStrA, const char* strB, size_t lenStrB)
//    {
//        if (lenStrA == lenStrB)
//        {
//            // alphabetical ordering amongst equal-length strings
//            return memcmp(strA, strB, lenStrA);
//        }
//        else
//        {
//            // longer strings before shorter strings
//            return (int)lenStrB - (int)lenStrA;
//        }
//    }
//
//    U32 WordCache::findPhrase(const char* str) const
//    {
//        const size_t strLen = strlen(str);
//
//        // Binary Search!
//        U32 minIdx = 0;
//        U32 maxIdx = mPhraseCount;
//        while (minIdx != maxIdx)
//        {
//            // look at string in middle
//            U32 searchIdx = minIdx + ((maxIdx - minIdx) >> 1);
//            const char* searchStr = getPhrase(searchIdx);
//            int cmp = CompareStrings(str, strLen, searchStr, mPhrasesLength[searchIdx]);
//            if (cmp == 0)
//            {
//                return searchIdx;
//            }
//            else if (cmp < 0)
//            {
//                // str would be earlier
//                maxIdx = searchIdx;
//            }
//            else
//            {
//                // str would be later
//                minIdx = searchIdx + 1;
//            }
//        }
//        return kPhraseNotFound;
//    }
//
//    U32 WordCache::getStarredDekoIndex(U32 phraseI, U32 dekoI) const
//    {
//        assertThatE(phraseI, isLessThan(mPhraseCount));
//        assertThatE(dekoI, isLessThan((U32)mDekosStarred[phraseI]));
//        return mDekosStart[phraseI] + dekoI;
//    }
//
//    U32 WordCache::getUnstarredDekoIndex(U32 phraseI, U32 dekoI) const
//    {
//        assertThatE(phraseI, isLessThan(mPhraseCount));
//        assertThatE(dekoI, isLessThan((U32)mDekosUnstarred[phraseI]));
//        return mDekosStart[phraseI] + mDekosStarred[phraseI] + dekoI;
//    }

}

#endif // __WORDCACHE_INL__