#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t* ptr;

sem_t bin;

static volatile int counter;

void thr_exit(int num){
    sem_post(&ptr[num]);
}

void* child(void* arg){
    int thread_no = *((int*)arg);
    printf("Child %d\n" , thread_no);
    for (int i=0 ; i<1e6 ; i++){
        sem_wait(&bin);
        counter++;
        sem_post(&bin);
    }
    thr_exit(thread_no);
    return NULL;
}

int main(int argc , char* argv[]){
    if (argc < 2){
        fprintf(stderr , "Additional Arguements are missing!!!\n");
        exit(1);
    }
    int Threads = atoi(argv[1]);
    ptr = (sem_t*)malloc(Threads*sizeof(sem_t));
    for (int i=0 ; i<Threads ; i++){
        sem_init(&ptr[i] , 0 , 0);
    }
    sem_init(&bin , 0  , 1);
    int temp[Threads];
    pthread_t thr[Threads];

    for (int i=0 ; i<Threads ; i++){
        temp[i] = i;
        pthread_create(&thr[i] , NULL , child , &temp[i]);
    }

    for (int i=0 ; i<Threads ; i++){
        sem_wait(&ptr[i]);
    }
    printf("Parent\n");
    printf("Counter : %d\n", counter);
    free(ptr);
    return 0;
}