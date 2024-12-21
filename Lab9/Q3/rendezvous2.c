#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

sem_t cond[2];

void* func(void* arg){
    int id = *((int*)arg);
    printf("Child %d\n", id);
    
    sem_post(&cond[(id+1)%2]);
    sem_wait(&cond[id]);

    printf("Child %d here\n" , id);
    return NULL;
}

int main(){
    sem_init(&cond[0] , 0 , 0);
    sem_init(&cond[1] , 0 , 0);
    
    pthread_t threads[2];
    int id1 = 0;
    int id2 = 1;

    pthread_create(&threads[0] , NULL , func , &id1);
    pthread_create(&threads[1] , NULL , func , &id2);

    pthread_join(threads[0] , NULL);
    pthread_join(threads[1] , NULL);

    return 0;
}