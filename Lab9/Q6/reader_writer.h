#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct _rwlock_t{
    sem_t lock;
    sem_t writelock;
    int readers;
} rwlock_t;

void rwlock_init();

void rwlock_acquire_writelock();

void rwlock_release_writelock();

void rwlock_acquire_readlock();

void rwlock_release_readlock();
