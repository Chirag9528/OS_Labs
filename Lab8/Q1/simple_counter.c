#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
typedef struct __counter_t{
    int value;
} counter_t;

counter_t counter;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* increment(){
    for (int i=0 ; i<1e6 ; i++){
        pthread_mutex_lock(&lock);
        counter.value++;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(int argc , char* argv[]){
    int num = atoi(argv[1]);
    pthread_t threads[num];
    printf("main: begin (counter = %d)\n" , counter.value);
    
    for (int i = 0 ; i<num ; i++){
        pthread_create(&threads[i] , NULL , increment , NULL);
    }

    for (int i = 0 ; i<num ; i++){
        pthread_join(threads[i] , NULL);
    }

    printf("main: done (counter = %d)\n" , counter.value);
    return 0;
}


/*
    gcc simple_counter.c
    time taskset -c 0,1,2,3 ./a.out <no. of threads>
    0 , 1 , 2, 3 -> represents cpu cores number (it has total 8 cores numbered from 0 to 7)

*/