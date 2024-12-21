#include <stdio.h>
#include <stdlib.h>
#define node_size sizeof(free_h)
#define free_size sizeof(node_h)

typedef struct node_header{
	int size;
	long magic_number;
}node_h;

typedef struct free_header{
	int size;
	struct free_header* next;
}free_h;

void* heap[4096];

void* mymalloc(int size);

void myfree(void* ptr);





