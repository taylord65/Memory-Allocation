#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "malloc.c"

int main(void){

	printf("Test my_malloc\n\n");

	my_mallopt(BEST_FIT);

	my_malloc(3000);
	my_malloc(3600);
	my_malloc(500);
	my_malloc(3700);

	my_malloc(200);

/*	my_mallopt(FIRST_FIT);

	my_malloc(3000);
	my_malloc(3600);
	my_malloc(500);
	my_malloc(3700);

	my_malloc(200);*/


	//BEST_FIT will place the 200 segment after the 3700 segment
	//FIRST_FIT will place the 200 segment after the 3600 segment

	my_mallinfo();

}