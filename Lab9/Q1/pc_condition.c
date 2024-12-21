#include "pc_condition.h"

int B;
int N;

int* buffer;
int fill_ptr = 0;
int use_ptr = 0;
int count = 0;


void put(int value){
    buffer[fill_ptr] = value;
    fill_ptr = (fill_ptr + 1) % B;
    count++;
}

int get() {
    int tmp = buffer[use_ptr];
    use_ptr = (use_ptr + 1) % B;
    count--;
    return tmp;
}

int check(){
    return buffer[use_ptr];
}

pthread_cond_t empty , fill;
pthread_mutex_t mutex;

int quantity;
int* count_threads;

void* update_producers(void* arg){
    int thread_id = ((funcarg*)arg)->thread_id;
    for (int i=1 ; i<=1000 ; i++){
        pthread_mutex_lock(&mutex);
        while (count == B){
            pthread_cond_wait(&empty , &mutex);
        }
        put((thread_id+1)*i);
        pthread_cond_broadcast(&fill);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* update_consumers(void* arg){
    int thread_id = ((funcarg*)arg)->thread_id;
    while (1){   
        if (quantity == N*1000){
            printf("Exiting Gateway1: %d\n", thread_id);
            return NULL;
        }
        pthread_mutex_lock(&mutex);
        while (count == 0){
            if (quantity == N*1000){
                pthread_mutex_unlock(&mutex);
                printf("Exiting Gateway2: %d\n", thread_id);
                return NULL;
            }
            pthread_cond_wait(&fill , &mutex);
        }
        int tmp = check();
        if (tmp % (thread_id+1) == 0){
            printf("Thread %d : %d\n" , thread_id , get());
            count_threads[thread_id]++;
            quantity++;
        }
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}


int main(int argc , char* argv[]){
    if (argc < 3){
        fprintf(stderr , "Additional Arguements are missing!!!\n");
        exit(1);
    }

    N = atoi(argv[1]);
    B = atoi(argv[2]);

    buffer = (int*)malloc(B*sizeof(int));
    count_threads = (int*)malloc((N+1)*sizeof(int));
    for (int i=0 ; i<N ; i++){
        count_threads[i] = 0;
    }

    pthread_t producers[N];
    pthread_t consumers[N];

    funcarg* pro = (funcarg*)malloc(N*sizeof(funcarg));
    funcarg* con = (funcarg*)malloc(N*sizeof(funcarg));

    for (int i=0 ; i<N ; i++){
        pro[i].thread_id = i+1;
        pthread_create(&producers[i] , NULL , update_producers , &pro[i]);
    }

    for (int i=0 ; i<N ; i++){
        con[i].thread_id = i+1;
        pthread_create(&consumers[i] , NULL , update_consumers , &con[i]);
    }

    for (int i=0 ; i<N ; i++){
        pthread_join(producers[i] , NULL);
    }
    
    for (int i=0 ; i<N ; i++){
        pthread_join(consumers[i] , NULL);
    }

    // for (int i=1 ; i<=N ; i++){
    //     printf("Thread %d : %d\n", i , count_threads[i]);
    // }

    return 0;
}