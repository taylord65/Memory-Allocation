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
		return_pointer = allocFirstFit(size);
	}
	else{
		return_pointer = allocBestFit(size);
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
	printf("Total bytes allocated for memory %d\n Total bytes allocated for meta information %d\n Total free space %d\n", TOTAL_ALLOCATION, TOTAL_META_ALLOCATION, TOTAL_FREE_SPACE);
	//printf("Largest contiguous free space %d\n");

}

void* allocFirstFit(int size){
	int segment_size_in_heap;
	int size_difference;
	int allocated_segment = 1;
	void* return_pointer = NULL;
	char* pointer = heap_bottom; //Initialize pointer to heap bottom

	while(pointer <= heap_top){

		segment_size_in_heap = *((int*)(pointer + ALLOCATION_TAG));

		if(*pointer == allocated_segment || segment_size_in_heap < size){
			pointer += segment_size_in_heap + 2*ALLOCATION_TAG + 2*LENGTH_TAG;
		}
		else {

			size_difference = segment_size_in_heap - size;

			if(size_difference==0){
				write_to_heap(pointer,1,size);

				TOTAL_FREE_SPACE -= size;
				TOTAL_ALLOCATION += size;
				return pointer;
			}
			else if(size_difference > MIN_SPLIT){
				write_to_heap(pointer,1,size);
				displayMemorySegment(pointer);
				pointer += 2*ALLOCATION_TAG + 2*LENGTH_TAG + size; 
				write_to_heap(pointer,0,size_difference-(ALLOCATION_TAG+LENGTH_TAG));
				displayMemorySegment(pointer);

				TOTAL_ALLOCATION += size;
				TOTAL_META_ALLOCATION += 2*ALLOCATION_TAG + 2*LENGTH_TAG;
				TOTAL_FREE_SPACE -= size;
				return pointer;
			}
			else{
				pointer += segment_size_in_heap + 2*ALLOCATION_TAG + 2*LENGTH_TAG;
			}
		}
	}
	//Extend the heap
	void *previous_heap_top = sbrk(size + 2*ALLOCATION_TAG + 2*LENGTH_TAG + EXTRA_HEAP_SPACE);
	heap_top += (size + 2*LENGTH_TAG + 2*ALLOCATION_TAG + EXTRA_HEAP_SPACE);

	write_to_heap(previous_heap_top,1,size);
	write_to_heap(previous_heap_top + size + 2*ALLOCATION_TAG + 2*LENGTH_TAG,0,EXTRA_HEAP_SPACE); 

	TOTAL_ALLOCATION += size;
	TOTAL_META_ALLOCATION += 2*(2*ALLOCATION_TAG + 2*LENGTH_TAG);
	TOTAL_FREE_SPACE += EXTRA_HEAP_SPACE;

	return (void *)previous_heap_top;
	
}

void* allocBestFit(int size){
	
}