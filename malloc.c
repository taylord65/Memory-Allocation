#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include "malloc.h"

int _policy = FIRST_FIT;
int uninitialized = 1;

char* heap_bottom;
char* heap_top;
int TOTAL_ALLOCATION = 0;
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

	printf("Start Address: %d\n",(int)ptr);

	char allocation = *((char*)ptr);
	printf("Allocation: %d\n", allocation);
	ptr += ALLOCATION_TAG;

	int size = *((int*)ptr);
	printf("Size: %d\n",size);
	ptr += (LENGTH_TAG + size + LENGTH_TAG + ALLOCATION_TAG);

	printf("End Address: %d\n",(int)ptr);

	printf("Heap Bottom: %d Heap Top: %d\n\n",(int)heap_bottom, (int)heap_top );
}


void* my_malloc(int size){	

	void* return_pointer = NULL;

	if(TOTAL_FREE_SPACE+size > MAX_MEMORY){
		printf("Not enough room in memory\n");
		return return_pointer;
	}

	if(uninitialized){

		heap_bottom = (char*)sbrk(size + 2*ALLOCATION_TAG + 2*LENGTH_TAG + EXTRA_HEAP_SPACE + 2*ALLOCATION_TAG + 2*LENGTH_TAG);

		heap_top = heap_bottom + size + 2*ALLOCATION_TAG + 2*LENGTH_TAG + EXTRA_HEAP_SPACE + 2*ALLOCATION_TAG + 2*LENGTH_TAG;

		write_to_heap(heap_bottom,1,size);
		write_to_heap(heap_bottom + size + 2*ALLOCATION_TAG + 2*LENGTH_TAG,0,EXTRA_HEAP_SPACE); 

		printf("Initial write\n");
		displayMemorySegment(heap_bottom);
		printf("Extra heap space addition\n");
		displayMemorySegment(heap_bottom + size + 2*ALLOCATION_TAG + 2*LENGTH_TAG);

		TOTAL_ALLOCATION += size;
		TOTAL_FREE_SPACE += EXTRA_HEAP_SPACE;

		uninitialized = 0;

		return (void*)heap_bottom;
	}

	if(_policy==FIRST_FIT){
		return_pointer = allocFirstFit(size);
		return (void *)return_pointer;

	}
	else{
		return_pointer = allocBestFit(size);
		return (void *)return_pointer;	
	}

	printf("Error in memory allocation\n");
	return return_pointer;

}


void my_free(void *ptr)
{
	if(ptr==NULL){
		return;
	}

	char allocation = *((char*)ptr);
	ptr += ALLOCATION_TAG;
	int size1 = *((int*)ptr);

	if(allocation==1){
		printf("This segment is allocated");
	}
	else{
		ptr -= ALLOCATION_TAG;
		*((char*)ptr) = 0;
		ptr += ALLOCATION_TAG + LENGTH_TAG + size1 + LENGTH_TAG;
		*((char*)ptr) = 0;

		ptr += ALLOCATION_TAG;

		char next_allocation = *((char*)ptr);

		if(next_allocation==0){

			//MERGE THE NEXT SEGMENT
			ptr += ALLOCATION_TAG;
			int size2 = *((int*)ptr);
			ptr += LENGTH_TAG + size2;
			*((int*)ptr) = size1 + size2 + 2*ALLOCATION_TAG + 2*LENGTH_TAG;//in the second length tag store the new length
			ptr += LENGTH_TAG;
			*((char*)ptr) = 0;

			ptr-= (LENGTH_TAG + size2 + 2*ALLOCATION_TAG + 2*LENGTH_TAG + size1 + LENGTH_TAG);
			*((int*)ptr) = size1 + size2 + 2*ALLOCATION_TAG + 2*LENGTH_TAG; //new length in the beginning
			ptr -= ALLOCATION_TAG;

			TOTAL_FREE_SPACE += 2*ALLOCATION_TAG + 2*LENGTH_TAG;
		}

		ptr -= ALLOCATION_TAG;

		displayMemorySegment(ptr);

		char previous_allocation = *((char*)ptr);

		if(previous_allocation==0){
			//MERGE PREVIOUS SEGMENT

			ptr-= LENGTH_TAG;
			int size0 = *((int*)ptr);
			ptr -= (size0 + LENGTH_TAG + ALLOCATION_TAG);
			*((char*)ptr) = 0;

			ptr += ALLOCATION_TAG;
			*((int*)ptr) = size0 + size1 + 2*ALLOCATION_TAG + 2*LENGTH_TAG;
			ptr += LENGTH_TAG + size0 + size1 + 2*ALLOCATION_TAG + 2*LENGTH_TAG;
			*((int*)ptr) = size0 + size1 + 2*ALLOCATION_TAG + 2*LENGTH_TAG;

			TOTAL_FREE_SPACE += 2*ALLOCATION_TAG + 2*LENGTH_TAG;

		}

	}

}

void my_mallopt(int policy)
{
	_policy = policy;
}

void my_mallinfo()
{
	int allocated_segment = 1;
	char* pointer = heap_bottom;
	int segment_size_in_heap;
	int largest_contiguous_free_space_size = -1;

	while(pointer <= heap_top){

		segment_size_in_heap = *((int*)(pointer + ALLOCATION_TAG));

		if(*pointer == allocated_segment){
			pointer += segment_size_in_heap + 2*ALLOCATION_TAG + 2*LENGTH_TAG;
		}
		else{

			if(largest_contiguous_free_space_size == -1){
				largest_contiguous_free_space_size = segment_size_in_heap;
			}
			else if(segment_size_in_heap > largest_contiguous_free_space_size){
				largest_contiguous_free_space_size = segment_size_in_heap;
			}

			pointer += segment_size_in_heap + 2*ALLOCATION_TAG + 2*LENGTH_TAG;

		}
	}
	printf("my_mallinfo statistics\n");
	printf("Total bytes allocated for memory: %d\nTotal free space: %d\n", TOTAL_ALLOCATION,TOTAL_FREE_SPACE);	
	printf("Largest contiguous free space: %d\n", largest_contiguous_free_space_size);
}

void* allocBestFit(int size){
	int allocated_segment = 1;
	int segment_size_in_heap;
	char* pointer = heap_bottom; 

	char* best_fit_address=NULL;
	int best_current_difference = -1;

	while(pointer <= heap_top){

		segment_size_in_heap = *((int*)(pointer + ALLOCATION_TAG));

		if(*pointer == allocated_segment || segment_size_in_heap < size){
			pointer += segment_size_in_heap + 2*ALLOCATION_TAG + 2*LENGTH_TAG;
		}
		else{

			if(best_current_difference == -1){
				//Set the first free segment's difference to the the best_current_difference
				best_current_difference = segment_size_in_heap - size;
				best_fit_address = pointer;	
			}
			else if(segment_size_in_heap - size < best_current_difference){
				best_current_difference = segment_size_in_heap - size;
				best_fit_address = pointer;	
			}

			pointer += segment_size_in_heap + 2*ALLOCATION_TAG + 2*LENGTH_TAG;

		}
	}

	if(best_current_difference==0){
		write_to_heap(best_fit_address,1,size);

		TOTAL_FREE_SPACE -= size;
		TOTAL_ALLOCATION += size;
		return best_fit_address;
	}
	else if(best_current_difference > MIN_SPLIT){
		write_to_heap(best_fit_address,1,size);
		displayMemorySegment(best_fit_address);
		best_fit_address += 2*ALLOCATION_TAG + 2*LENGTH_TAG + size; 
		write_to_heap(best_fit_address,0,best_current_difference-(2*(ALLOCATION_TAG+LENGTH_TAG)));
		displayMemorySegment(best_fit_address);

		TOTAL_ALLOCATION += size;
		TOTAL_FREE_SPACE -= size;
		return best_fit_address;
	}
	else {
	printf("##################Extend the heap##################\n\n");
	void *previous_heap_top = sbrk(size + 2*ALLOCATION_TAG + 2*LENGTH_TAG + EXTRA_HEAP_SPACE + 2*ALLOCATION_TAG + 2*LENGTH_TAG);
	heap_top += (size + 2*LENGTH_TAG + 2*ALLOCATION_TAG + EXTRA_HEAP_SPACE + 2*ALLOCATION_TAG + 2*LENGTH_TAG);

	write_to_heap(previous_heap_top,1,size);
	displayMemorySegment(previous_heap_top);
	write_to_heap(previous_heap_top + size + 2*ALLOCATION_TAG + 2*LENGTH_TAG,0,EXTRA_HEAP_SPACE); 
	displayMemorySegment(previous_heap_top + size + 2*ALLOCATION_TAG + 2*LENGTH_TAG);

	TOTAL_ALLOCATION += size;
	TOTAL_FREE_SPACE += EXTRA_HEAP_SPACE;

	return (void *)previous_heap_top;	
	}
}

void* allocFirstFit(int size){
	int segment_size_in_heap;
	int size_difference;
	int allocated_segment = 1;
	void* return_pointer = NULL;
	char* pointer = heap_bottom; 

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
				write_to_heap(pointer,0,size_difference-(2*(ALLOCATION_TAG+LENGTH_TAG)));
				displayMemorySegment(pointer);

				TOTAL_ALLOCATION += size;
				TOTAL_FREE_SPACE -= size;
				return pointer;
			}
			else{
				pointer += segment_size_in_heap + 2*ALLOCATION_TAG + 2*LENGTH_TAG;
			}
		}
	}
	//Extend the heap
	printf("##################Extend the heap##################\n\n");
	void *previous_heap_top = sbrk(size + 2*ALLOCATION_TAG + 2*LENGTH_TAG + EXTRA_HEAP_SPACE + 2*ALLOCATION_TAG + 2*LENGTH_TAG);
	heap_top += (size + 2*LENGTH_TAG + 2*ALLOCATION_TAG + EXTRA_HEAP_SPACE+ 2*ALLOCATION_TAG + 2*LENGTH_TAG);

	write_to_heap(previous_heap_top,1,size);
	displayMemorySegment(previous_heap_top);
	write_to_heap(previous_heap_top + size + 2*ALLOCATION_TAG + 2*LENGTH_TAG,0,EXTRA_HEAP_SPACE); 
	displayMemorySegment(previous_heap_top + size + 2*ALLOCATION_TAG + 2*LENGTH_TAG);

	TOTAL_ALLOCATION += size;
	TOTAL_FREE_SPACE += EXTRA_HEAP_SPACE;

	return (void *)previous_heap_top;	
}