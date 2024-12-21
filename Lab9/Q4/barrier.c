#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
int N;

static volatile int count;
pthread_cond_t var;
pthread_mutex_t lockk;

void barrier(){
    pthread_mutex_lock(&lockk);
    count++;

    if (count != N){
        pthread_cond_wait(&var , &lockk);
    }
    else{
        pthread_cond_broadcast(&var);
    }
    pthread_mutex_unlock(&lockk);
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