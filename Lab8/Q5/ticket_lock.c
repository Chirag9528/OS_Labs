#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>

static volatile int counter = 0;

pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

typedef struct __loct_t{
    int ticket;
    int turn;
}lock_t;

void lock_inti(lock_t *lock){
    lock->ticket = 0;
    lock->turn = 0;
    return;
}

void lock(lock_t* lock){
    int myturn = atomic_fetch_add(&lock->ticket , 1);
    while (lock->turn != myturn); // spin
}

void unlock(lock_t* lock){
    lock->turn = lock->turn + 1;
}

lock_t ticket_lock;

void* update(void* arg){
    for (int i=0 ; i<1e6 ; i++){
        lock(&ticket_lock);
        // pthread_mutex_lock(&lock2);
        counter++;
        unlock(&ticket_lock);
        // pthread_mutex_unlock(&lock2);
    
    }
    return NULL;
}


int main(){

    printf("counter: begin: %d\n" , counter);
    pthread_t threads[4];
    lock_inti(&ticket_lock);
    for (int i=0 ; i<4 ; i++){
        pthread_create(&threads[i] , NULL , update , NULL);
    }

    for (int i=0 ; i<4 ; i++){
        pthread_join(threads[i] , NULL);
    }
    printf("counter: end: %d\n" , counter);

    return 0;
}
