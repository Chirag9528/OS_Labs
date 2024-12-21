#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct __precise{
    int value;
} precise_t;


pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

precise_t counter;

void* increment();
