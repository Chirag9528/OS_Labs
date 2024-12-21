#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int NUMCPUS;
typedef struct __counter_t{
    int global;
    pthread_mutex_t glock;
    int* local;
    pthread_mutex_t* llocks;
    int threshold;
}counter_t;

typedef struct __func_arg{
    counter_t* counter;
    int thread_id;
    int amount;
}func_arg;

void init(counter_t* c , int threshold);

void* update(void* temp);

int get(counter_t* c);

