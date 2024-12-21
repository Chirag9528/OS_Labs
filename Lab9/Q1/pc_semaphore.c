#include "pc_semaphore.h"

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

int quantity;
sem_t empty;
sem_t full;
sem_t mutex;

int terminate_consumers;

void* update_producers(void* arg){
    int thread_id = ((funcarg*)arg)->thread_id;
    for (int i=1 ; i<= 1000 ; i++){
        sem_wait(&empty);
        sem_wait(&mutex);
        put((thread_id+1)*i);
        sem_post(&mutex);
        sem_post(&full);
    }
    return NULL;
}

void* update_consumers(void* arg){
    int thread_id = ((funcarg*)arg)->thread_id;
    while (1){
        if (quantity == N*1000){
            terminate_consumers = 1;
            sem_post(&full); // waking other consumer
            printf("Exiting Gateway1: %d\n", thread_id);
            return NULL;
        }
        sem_wait(&full);
        sem_wait(&mutex);
        if (terminate_consumers){
            sem_post(&mutex);
            sem_post(&full); // waking other consumer
            printf("Exiting Gateway2: %d\n", thread_id);
            return NULL;
        }
        int tmp = check();
        int flag = 0;
        if (tmp % (thread_id+1) == 0){
            printf("Thread %d : %d\n" , thread_id , get());
            quantity++;
            flag = 1;
        }
        sem_post(&mutex);

        if (flag){
            sem_post(&empty);
        }
        else{
            sem_post(&full);
        }
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

    pthread_t producers[N];
    pthread_t consumers[N];

    funcarg* pro = (funcarg*)malloc(N*sizeof(funcarg));
    funcarg* con = (funcarg*)malloc(N*sizeof(funcarg));

    sem_init(&mutex , 0 , 1);
    sem_init(&full , 0 , 0);
    sem_init(&empty , 0 , B);

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
    printf("Quantity: %d\n", quantity);

    return 0;
}