# Word Cache

####Overview

Word cache is a system I created for the [Doki-Doki Mail](https://www.youtube.com/watch?v=H7hFijr5v-c) application.  In it, as a user typed a messaged the application would highlight relavant words that could be decorated by the system.  The user could then click on a highlighted word and see all the images that could be used in place of the word.  These matches could be multi-word phrases like 'Hi you guys' or single words, or letters of the alphabet/punctuation.

Word cache was a data structure that held the dictionaries and references to various art assets for each word.  At the core of the word cache was a `FindMatches` function.  This would take a string and highlight portions of it which match to entries to the word cache.  The algorithm runs as follows:

1. First it copies the incoming text to a buffer so it can be manipulated.
1. Then it translates the unicode string into a set of indexes so the results refer to the actual index of a character, not the character index in the char buffer. 
1. All phrases in the word cache are iterated from longest to shortest
1. For each phrase, we do a couple of checks:
    1. Is the phrase in the string?
    1. Is it a whole word match?  Reject contractions.  `Can't` holds `can`, so we should reject a match here.
    1. Single character matches always pass
1. If all these conditions pass, store match information
1. Replace the matching phrase in the working buffer with invalid characters, like `*`

That's it!  It's O(N*M) speed and O(M) memory where N is the length of the string and M is the number of words in the dictionary.  A bit brute force.  When I first wrote the algorithm, I had intended to optimize it later as needed.  In the end it wasn't necessary because our dictinory was only around 10k words, and this algorithm's performance was satisfactory.
 
####Some thoughts on improvements

* As it stands, the system does exact matching.  Having some sort of 'soft' matching would have helped reduce the size of the data set.
* There could be some quick optimizations to do early exits when certain words/phrases would no longer match.

####A Trie Implementation
Had the dictionary been much larger, and performace needed improvement I would have likely used a [trie](https://en.wikipedia.org/wiki/Trie) to store the word cache instead of a map.  This would have drastically sped up the time to find a match.  In this system the algorithm would:
1. iterate through each character of the string.  
1. As it does so it would also walk the trie with each matching character.  
1. It would continue to walk the string until a terminating trie node is found(meaning a match is found) or there are no more nodes to walk in the trie.  
1. At this point the match(if found) would be recorded and we would reset the trie iterator and continue iterating the source string.
Clearly this would be a great improvement, as now it is just O(N) where N is the length of the string.  Memory usage might go up as each character would need a list of nodes, but I suspect it would be worth the trade-off.

####Summaries of the files in this folder

* [WordCache.h](WordCache.h)/[cpp](WordCache.cpp) is a simplified version of the word cache, used to demonstrate the `FindMatches` function.
* [Utf8.h](Utf8.h)/[inl](Utf8.inl) is a helper class for translating UTF-8 character strings.
* [WordCachePch.h](WordCachePch.h) is a helper file to include dependencies for the example.
* The word cache sample here is a stripped down version of the one that shipped. The [Production](Production/) folder contains the fuller version.  A lot of the pipeline code was written by others, but I included it in case you wanted to see what it looked like in the fuller context of things.
