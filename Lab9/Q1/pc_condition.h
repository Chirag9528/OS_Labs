#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void put(int value);

int get();

int check();

void* update_producers(void* arg);

void* update_consumers(void* arg);

typedef struct func_args{
    int thread_id;
}funcarg;