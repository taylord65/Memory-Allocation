malloc: malloc.o
	gcc malloc.o -o Dotsikas_Taylor_malloc

malloc.o: malloc.h 
	gcc -c malloc_test.c -o malloc.o

clean:
	-rm -f malloc.o
	-rm -f Dotsikas_Taylor_malloc