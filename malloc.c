#include "mymalloc.h"

char *my_malloc_error;
int _policy = FIRST_FIT;
int need_init = 1;
void* first;

void init()
{
	first = sbrk(0);
}
void* my_malloc(int size)
{
	

}


void my_free(void *ptr)
{
	
}

void my_mallopt(int policy)
{
	_policy = policy;
}

void my_mallinfo()
{

}


void* allocFirstFreeSpaceIndex(int size)
{
	
}

void* allocFitFreeSpaceIndex(int size)
{
	
}

void* getPrev(void* pointer)
{
	
}

Allocation getAllocation(void *pointer)
{
	
}