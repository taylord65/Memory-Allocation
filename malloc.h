#include <stdint.h>

#define MAX_MEMORY 131072 
#define ALLOCATION_TAG 1
#define LENGTH_TAG 4
#define EXTRA_HEAP_SPACE 4000 
#define MIN_SPLIT 6


void* my_malloc(int size);
void my_free(void *ptr);
void my_mallopt(int policy);
void my_mallinfo();
void* allocFirstFit(int size);
void* allocBestFit(int size);


//Helper functions
void write_to_heap(void* start_address, char allocation, int size);
void displayMemorySegment(void* pointer);


extern char *my_malloc_error;
extern int _policy;
