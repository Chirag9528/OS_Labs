#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct __Zem_t{
    int value;
    pthread_cond_t cond;
    pthread_mutex_t lock;
}Zem_t;

void Zem_init(Zem_t* s , int value);

void Zem_wait(Zem_t* s);

void Zem_post(Zem_t* s);