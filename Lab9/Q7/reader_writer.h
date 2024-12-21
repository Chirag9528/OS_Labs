#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

typedef struct __rwlock_t{
    bool hasWriter;
    int readercount;
    pthread_cond_t cond;
    pthread_mutex_t lock;
}rwlock_t;

void rwlock_init();

void rwlock_acquire_writelock();

void rwlock_release_writelock();

void rwlock_acquire_readlock();

void rwlock_release_readlock();
