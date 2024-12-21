#include "linked_list_hand.h"

void init_per_node_lock(ll_per_node_lock* list){
    list->head = (per_node_lock*)malloc(sizeof(per_node_lock));
    list->head->value = -1;
    list->head->next = NULL;
    pthread_mutex_init(&list->head->nlock , NULL);
}

void print_per_node_lock(per_node_lock* head){
    printf("Linked List is: \n");
    while (head != NULL){
        pthread_mutex_lock(&head->nlock);
        printf("%d ", head->value);
        head = head->next;
        pthread_mutex_unlock(&head->nlock);
    }
    printf("\n");
    return;
}

void insert_per_node_lock(ll_per_node_lock* list , int value){
    per_node_lock* new_node = (per_node_lock*)malloc(sizeof(per_node_lock));
    if (new_node == NULL){
        perror("malloc");
        return;
    }
    pthread_mutex_lock(&list->lock);
    
    new_node->value = value;
    new_node->next = list->head->next;
    pthread_mutex_init(&new_node->nlock , NULL);
    list->head->next = new_node;

    pthread_mutex_unlock(&list->lock);

    return;
}

int List_Lookup_per_node_lock(ll_per_node_lock* list , int key){
    int rv = -1;

    pthread_mutex_lock(&list->lock);
    per_node_lock* curr = list->head;
    
    pthread_mutex_lock(&list->head->nlock);
    pthread_mutex_unlock(&list->lock);

    while (curr){
        if (curr->value == key){
            rv = 0;
            break;
        }
        per_node_lock* prev = curr;
        curr = curr->next;
        if (curr){
            pthread_mutex_lock(&curr->nlock);
        }
        pthread_mutex_unlock(&prev->nlock);
    }
    return rv;
}


void free_node(per_node_lock* nde){
    if (nde == NULL) return;
    free_node(nde->next);
    free(nde);
}

void free_list(ll_per_node_lock** list){
    if (list == NULL || *list == NULL) return;
    free_node((*list)->head);
    free(*list);
    *list = NULL;
}

void* update_per_node_lock(void* arg){
    int* i = (int*)arg;
    for (int j = 0 ; j< (*i) ; j++){
        int num = rand()%2;
        if (num){
            List_Lookup_per_node_lock(list , -2);
        }
        else{
            insert_per_node_lock(list , j);
        }
    }
}

int main(){
    FILE* fp2 = fopen("data3.csv" , "w");
    if (fp2 == NULL){
        fprintf(stderr, "File opening failed.\n");
        exit(1);
    }
    fprintf(fp2 , "x,y\n");
    for (int i=1; i<=16384 ; i+= 100){

        list = (ll_per_node_lock*)malloc(sizeof(ll_per_node_lock));
        init_per_node_lock(list);
        pthread_t threads[4];
        int num = i/4;

        clock_t start = clock();
        for (int thread = 0 ; thread <4 ; thread++){
            pthread_create(&threads[thread] , NULL , update_per_node_lock , &num);
        }

        for (int thread = 0; thread < 4 ; thread++){
            pthread_join(threads[thread] , NULL);
        }
        clock_t end = clock();

        free_list(&list);
        double time = (double)(end-start);
        fprintf(fp2 , "%d,%f\n" , i , time);
    }

    return 0;
}