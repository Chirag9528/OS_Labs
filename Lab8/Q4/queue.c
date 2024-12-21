#include "queue.h"

void init(){
    node* tmp = (node*)malloc(sizeof(node));
    tmp->next = NULL;
    q->head = tmp;
    q->tail = tmp;
}

void enqueue(int value){
    node* new_node = (node*)malloc(sizeof(node));
    if (new_node == NULL){
        fprintf(stderr, "Malloc Failed!!!\n");
        exit(0);
    }
    new_node->value = value;
    new_node->next = NULL;
    q->tail->next = new_node;
    q->tail = new_node;
    return;
}

int dequeue(int* value){
    if (q->head->next == NULL){
        return -1;
    }
    node* tmp = q->head;
    node* new_head = tmp->next;
    *value = new_head->value;
    q->head = new_head;
    free(tmp);
    return 0;
}


int main(){
    FILE* fp = fopen("data1.csv" , "w");
    if (fp == NULL){
        fprintf(stderr , "File not Opened Error!!!\n");
        exit(1);
    }
    fprintf(fp , "x,y\n");
    for (int i=1 ; i<=16384 ; i+=100){
        q = (queue*)malloc(sizeof(queue));
        init();
        clock_t start = clock();
        for (int j = 0; j<i ; j++){
            int num = rand()%2;
            if (num){
                enqueue(j);
            }
            else{
                int val;
                dequeue(&val);
            }
        }
        clock_t end = clock();
        double time = (double)(end-start)/CLOCKS_PER_SEC;
        fprintf(fp , "%d,%f\n" , i , time);
    }

    return 0;
}