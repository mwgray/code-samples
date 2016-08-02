# Compact String

Compact String is a pretty handy optimization for string lookups/comparison.  It basically calculates the strings checksum on assignment and discards the character data.  This is mainly useful for strings that are only used for comparison, like event signatures or file handles.  Using this system grants a significant savings to performance and to memory.

A larger use case example for this class is for indexing a giant file blob.  You can store a file system in an entire blob and use compact strings as keys to a map of data offsets.  I used this on a Nintendo DS, where traditional file system calls are not available in certain scenarios.

Summaries of the files: 

* (CompactString.h)[CompactString.h] is just a pre-processor controlled typedef to determine which variant of the string to use based on the compile target.
