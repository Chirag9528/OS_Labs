#include "queue_lock.h"

void init(){
    node* tmp = (node*)malloc(sizeof(node));
    tmp->next = NULL;
    q->head = tmp;
    q->tail = tmp;
    pthread_mutex_init(&q->head_lock, NULL);
    pthread_mutex_init(&q->tail_lock, NULL);
}

void enqueue(int value){
    node* new_node = (node*)malloc(sizeof(node));
    if (new_node == NULL){
        fprintf(stderr, "Malloc Failed!!!\n");
        exit(0);
    }
    new_node->value = value;
    new_node->next = NULL;

    pthread_mutex_lock(&q->tail_lock);
    q->tail->next = new_node;
    q->tail = new_node;
    pthread_mutex_unlock(&q->tail_lock);
    return;
}

int dequeue(int* value){
    pthread_mutex_lock(&q->head_lock);
    if (q->head->next == NULL){
        pthread_mutex_unlock(&q->head_lock);
        return -1;
    }
    node* tmp = q->head;
    node* new_head = tmp->next;
    *value = new_head->value;
    q->head = new_head;
    pthread_mutex_unlock(&q->head_lock);
    free(tmp);
    return 0;
}

void* update(void* arg){
    int* i = (int*)arg;
    for (int j = 0; j<(*i) ; j++){
        int num = rand()%2;
        if (num){
            enqueue(j);
        }
        else{
            int val;
            dequeue(&val);
        }
    }
    return NULL;
}


int main(){
    FILE* fp = fopen("data2.csv" , "w");
    if (fp == NULL){
        fprintf(stderr , "File not Opened Error!!!\n");
        exit(1);
    }
    fprintf(fp , "x,y\n");
    for (int i=1 ; i<=16384 ; i+=100){
        q = (queue*)malloc(sizeof(queue));
        init();

        pthread_t threads[4];
        int num = i/4;
        clock_t start = clock();
        for (int thread = 0 ; thread < 4 ; thread++){
            pthread_create(&threads[thread] , NULL , update , &num);
        }

        for (int thread = 0 ; thread < 4 ; thread++){
            pthread_join(threads[thread] , NULL);
        }

        clock_t end = clock();
        double time = (double)(end-start)/CLOCKS_PER_SEC;
        fprintf(fp , "%d,%f\n" , i , time);
    }

    return 0;
}