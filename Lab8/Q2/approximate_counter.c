#include "approximate_counter.h"

void init(counter_t* c , int threshold){
    c->threshold = threshold;
    c->global = 0;
    pthread_mutex_init(&c->glock , NULL);
    c->local = (int*)malloc(NUMCPUS*sizeof(int));
    c->llocks = (pthread_mutex_t*)malloc(NUMCPUS*sizeof(pthread_mutex_t));
    for (int i=0 ; i<NUMCPUS ; i++){
        c->local[i] = 0;
        pthread_mutex_init(&c->llocks[i] , NULL);
    }
}

void* update(void* temp){
    func_arg* arg_list = (func_arg*)temp;
    counter_t* c = arg_list->counter;
    int threadID = arg_list->thread_id;
    int amt = arg_list->amount;
    int cpu = threadID % NUMCPUS;
    for (int i=0 ; i<1e6 ; i++){
        pthread_mutex_lock(&c->llocks[cpu]);
        c->local[cpu]+= amt;
        if (c->local[cpu] >= c->threshold){
            pthread_mutex_lock(&c->glock);
            c->global += c->local[cpu];
            pthread_mutex_unlock(&c->glock);
            c->local[cpu] = 0;
        }
        pthread_mutex_unlock(&c->llocks[cpu]);
    }
    return NULL;
}

int get(counter_t* c){
    pthread_mutex_lock(&c->glock);
    int val = c->global;
    pthread_mutex_unlock(&c->glock);
    return val;
}



int main(int argc , char* argv[]){
    if (argc < 4){
        fprintf(stderr , "Additional arguements are missing!!!\n");
        return -1;
    }
    NUMCPUS = atoi(argv[1]);
    int threshold = atoi(argv[2]);
    int num_threads = atoi(argv[3]);

    counter_t counter;
    init(&counter , threshold);
    
    printf("Starting Counter: %d\n" , counter.global);
    clock_t start = clock();

    pthread_t threads[num_threads];

    for (int i=0 ; i<num_threads ; i++){
        func_arg argmts;
        argmts.amount = 1;
        argmts.counter = &counter;
        argmts.thread_id = i;
        pthread_create(&threads[i] , NULL , update , &argmts);
    }

    for (int i=0 ; i<num_threads ; i++){
        pthread_join(threads[i] , NULL);
    }

    clock_t end = clock();

    printf("Ending Counter: %d\n" , counter.global);
    printf("Time Taken: %ld\n", (end-start)/CLOCKS_PER_SEC);

    return 0;
}