#include "pc_semaphore.h"
#include "MySemaphore.h"

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
Zem_t empty;
Zem_t full;
Zem_t mutex;

int terminate_consumers;

void* update_producers(void* arg){
    int thread_id = ((funcarg*)arg)->thread_id;
    for (int i=1 ; i<= 1000 ; i++){
        Zem_wait(&empty);
        Zem_wait(&mutex);
        put((thread_id+1)*i);
        Zem_post(&mutex);
        Zem_post(&full);
    }
    return NULL;
}

void* update_consumers(void* arg){
    int thread_id = ((funcarg*)arg)->thread_id;
    while (1){
        if (quantity == N*1000){
            terminate_consumers = 1;
            Zem_post(&full); // waking other consumer
            printf("Exiting Gateway1: %d\n", thread_id);
            return NULL;
        }
        Zem_wait(&full);
        Zem_wait(&mutex);
        if (terminate_consumers){
            Zem_post(&mutex);
            Zem_post(&full); // waking other consumer
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
        Zem_post(&mutex);

        if (flag){
            Zem_post(&empty);
        }
        else{
            Zem_post(&full);
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

    Zem_init(&mutex , 1);
    Zem_init(&full , 0);
    Zem_init(&empty , B);

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