#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int N;

static volatile int count;

sem_t var;
sem_t lockk;

void barrier(){
    sem_wait(&lockk);
    count++;
    sem_post(&lockk);

    while (count != N){
        sem_wait(&var);
    }
    sem_post(&var);
}

void* update(void* arg){
    int thread_id = *((int*)arg);
    printf("Child: %d\n" , thread_id);

    barrier();

    printf("Child Exiting: %d\n" , thread_id);

    return NULL;
}


int main(int argc , char* argv[]){
    if (argc < 2){
        fprintf(stderr, "Additional arguements missing!!!\n");
        exit(1);
    }
    N = atoi(argv[1]);
    sem_init(&var , 0 , 0);
    sem_init(&lockk , 0 , 1);

    pthread_t threads[N];
    int ids[N];
    for (int i=0 ; i<N ; i++){
        ids[i] = i;
        pthread_create(&threads[i] , NULL , update,&ids[i]);
    }

    for (int i=0 ; i<N ; i++){
        pthread_join(threads[i] , NULL);
    }

    return 0;
}