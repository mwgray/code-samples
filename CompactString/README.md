# Compact String

Compact String is a pretty handy optimization for string lookups/comparison.  It basically calculates the strings checksum on assignment and discards the character data.  This is mainly useful for strings that are only used for comparison, like event signatures or file handles.  Using this system grants a significant savings to performance and to memory.

A larger use case example for this class is for indexing a giant file blob.  You can store a file system in an entire blob and use compact strings as keys to a map of data offsets.  I used this on a Nintendo DS project, where traditional file system calls are not available in certain scenarios.  Using compact strings saved a significant amount of memory, as well as sped up load times.  

Summaries of the files in this folder: 

* [CompactStringRelease.h](CompactStringRelease.h)/[cpp](CompactStringRelease.cpp) is the simpler version of compact string. It calculate the checksum on assignment and discards string data.
* [CompactStringDebug.h](CompactStringDebug.h)/[cpp](CompactStringDebug.cpp) is the a more comple version. It performs the same as the release variant, but keeps the string data around.  To save additional memory when keeping string data around, I use a map from checksum to string data.  This eliminates storing the same string more than once. It also does some validation and checks for hash collisions.
* [CompactString.h](CompactString.h) is just a pre-processor controlled typedef to determine which variant of the string to use based on the compile target.
* [FNV1.h](FNV1.h)/[cpp](FNV1.cpp) is the hashing algorithm used to calculate the checksum
