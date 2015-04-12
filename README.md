Memory Allocation Package
Taylor Dotsikas

This is a boundary tag scheme implementation of the malloc function. Each segment of data has an allocation tag and a length tag on either side. The allocation tag is 1 byte, the length tag is 4 bytes. Segments are writtent to the heap using a pointer that was assigned using the sbrk() system call upon initialization. Allocation: 1 is occupied, 0 is free. Increasing the program break to fit a new segment will also allocate a free space segment of size 4000 to reduce sbrk() calls.

In the malloc_test file you can specify the policy, either BEST_FIT or FIRST_FIT. Either one can be passed to the my_mallopt() function to use it as a memory allocation scheme. Anytime a segment is written to the heap, the remaining bytes available will be setup as free space. BEST_FIT will search for the free memory segment that will leave the smallest free space segment when it is written. FIRST_FIT will search for the first free memory segment that will hold the required size. 

A size difference needs to be larger than the minimum size so a free space segment has the ability to hold at least 1 byte. 

Passing a size parameter to my_malloc will write it to the heap, the command prompt will display statistics about the location of the segment and size. 

From running malloc_test.c you can see from the display statistics that all the segments fit together. The start addresses and end addresses are displayed. 

In the main function, if the policy is set to FIRST_FIT, the 200 size segment will be written after the 3600 segment. If the policy is BEST_FIT the 200 size segment will be placed after the 3700 segment. This confirms that both policies perform correctly. 

my_mallinfo() displays the required statistics. 

The my_free() function can free a segment and also merge the previous, next, or both to it if they are free segments. 