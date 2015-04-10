#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include "malloc.h"

//int _policy = FIRST_FIT;
int uninitialized = 1;

char* heap_bottom;
char* heap_top;
int TOTAL_ALLOCATION = 0;
int TOTAL_META_ALLOCATION = 0;
int TOTAL_FREE_SPACE = 0;

void write_to_heap(void* start_address, char allocation, int size){

	void *pointer = start_address;

	*((char*)pointer)= allocation;
	pointer += ALLOCATION_TAG;

	*((int*)pointer)= size; 
	pointer += (LENGTH_TAG + size); 

	*((int*)pointer) = size;
	pointer += LENGTH_TAG;

	*((char*)pointer) = allocation;

}

void displayMemorySegment(void* pointer){
	void *ptr = pointer;

	printf("Start Address %d\n",(int)ptr);

	char allocation = *((char*)ptr);
	printf("Allocation: %d\n", allocation);
	ptr += ALLOCATION_TAG;

	int size = *((int*)ptr);
	printf("Size: %d\n",size);
	ptr += (LENGTH_TAG + size);

	printf("Heap Bottom: %d Heap Top: %d\n",(int)heap_bottom, (int)heap_top );

}


void* my_malloc(int size){

	void* return_pointer = NULL;

	if(uninitialized){
		//There is no room in the heap
		//sbrk will return void pointer to start of newly allocated space, cast to char pointer to store in heap_bottom 

		heap_bottom = (char*)sbrk(size + 2*ALLOCATION_TAG + 2*LENGTH_TAG + EXTRA_HEAP_SPACE);

		heap_top = heap_bottom + size + 2*ALLOCATION_TAG + 2*LENGTH_TAG + EXTRA_HEAP_SPACE + 2*ALLOCATION_TAG + 2*LENGTH_TAG;

		write_to_heap(heap_bottom,1,size);
		write_to_heap(heap_bottom + size + 2*ALLOCATION_TAG + 2*LENGTH_TAG,0,EXTRA_HEAP_SPACE); 

		printf("Initial write\n");
		displayMemorySegment(heap_bottom);
		printf("Extra heap space addition\n");
		displayMemorySegment(heap_bottom + size + 2*ALLOCATION_TAG + 2*LENGTH_TAG);

		TOTAL_ALLOCATION += size;
		TOTAL_META_ALLOCATION += 2*(2*ALLOCATION_TAG + 2*LENGTH_TAG);
		TOTAL_FREE_SPACE += EXTRA_HEAP_SPACE;

		uninitialized = 0;

		return (void*)heap_bottom;
	}

/*	if(_policy==FIRST_FIT){
		return_pointer = allocFirstFit();
	}
	else{
		return_pointer = allocBestFit();
	}
*/
	printf("Error in memory allocation\n");
	return return_pointer;

}


void my_free(void *ptr)
{
	
}

void my_mallopt(int policy)
{
	//_policy = policy;
}

void my_mallinfo()
{
	//printf("Total bytes allocated for memory %d\n Total bytes allocated for meta information %d\n Total free space %d\n");
//	printf("Largest contiguous free space %d\n");

}

void* getSegmentSize(void* pointer){
	//return
}

void* allocFirstFit(int size){

/*	int allocated = 1;

	void* return_pointer = NULL;
	char* pointer = heap_bottom; //Initialize pointer to heap bottom

	if(*pointer == allocated){
		pointer += getSegmentSize(pointer);
	}
	else {
		//This is a free segment
		if()
	}
*/
	
}

void* allocBestFit(int size)
{
	
}

void* getPrev(void* pointer)
{
	
}
