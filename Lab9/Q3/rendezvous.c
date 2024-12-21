#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include<unistd.h>

static volatile int count;
sem_t bin;
sem_t cond;

sem_t for_main;

void* func(void* arg){
    int id = *((int*)arg);
    printf("I am a child : %d\n", id);
        
    sem_wait(&bin);
    count++;
    sem_post(&bin);

    sem_post(&for_main);
    sem_wait(&cond);

    printf("Hi i am Child %d\n", id);
    return NULL;
}

int main(){
    pthread_t threads[2];
    sem_init(&bin , 0 , 1);
    sem_init(&cond , 0 , 0);

    int id1 = 0;
    int id2 = 1;

    pthread_create(&threads[0] , NULL , func , &id1);
    pthread_create(&threads[1] , NULL , func , &id2);

    while (count != 2){
        sem_wait(&for_main);
    }

    sem_wait(&bin);
    sem_post(&cond);
    sem_post(&cond);
    sem_post(&bin);

    pthread_join(threads[0] , NULL);
    pthread_join(threads[1] , NULL);

    return 0;
}