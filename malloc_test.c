#include <stdio.h>
#include "malloc.h"
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main(void){
	printf("Test my_malloc\n");

	my_mallopt(FIRST_FIT);

	my_malloc(3000);
	my_malloc(500);
	my_malloc(700);	
	my_malloc(600);
	my_malloc(5000);
	my_malloc(100);



	my_mallinfo();

}