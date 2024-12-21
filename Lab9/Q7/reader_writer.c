 #include "reader_writer.h"
#include "linked_list.h"

rwlock_t* rw;
int write_count;
int read_count;

void rwlock_init(){
    rw->hasWriter = false;
    rw->readercount = 0;
    pthread_mutex_init(&rw->lock , NULL);
    pthread_cond_init(&rw->cond , NULL);
    return;
}

void rwlock_acquire_writelock(){
    pthread_mutex_lock(&rw->lock);
    while (rw->hasWriter){
        pthread_cond_wait(&rw->cond , &rw->lock);
    }
    rw->hasWriter = true;
    while (rw->readercount > 0){
        pthread_cond_wait(&rw->cond , &rw->lock);
    }
    pthread_mutex_unlock(&rw->lock);
}

void rwlock_release_writelock(){
    pthread_mutex_lock(&rw->lock);
    rw->hasWriter = false;
    pthread_cond_broadcast(&rw->cond);
    pthread_mutex_unlock(&rw->lock);
}

void rwlock_acquire_readlock(){
    pthread_mutex_lock(&rw->lock);
    while (rw->hasWriter){
        pthread_cond_wait(&rw->cond , &rw->lock);
    }
    rw->readercount++;
    pthread_mutex_unlock(&rw->lock);
}

void rwlock_release_readlock(){
    pthread_mutex_lock(&rw->lock);
    rw->readercount--;
    if (rw->readercount == 0){
        pthread_cond_broadcast(&rw->cond);
    }
    pthread_mutex_unlock(&rw->lock);
}

void init(){
    list->head = (node*)malloc(sizeof(node));
    list->head->value = -1;
    list->head->next = NULL;
    return;
}

void print(node* head){
    printf("Linked List is: \n");
    while (head != NULL){
        printf("%d ", head->value);
        head = head->next;
    }
    printf("\n");
    return;
}

void insert(int value){
    node* new_node = (node*)malloc(sizeof(node));
    if (new_node == NULL){
        perror("malloc");
        return; 
    }
    rwlock_acquire_writelock();
    new_node->value = value;
    new_node->next = list->head;
    list->head = new_node;
    write_count++;
    printf("Write: %d\n" , write_count);
    rwlock_release_writelock();
    return;
}

int List_Lookup(int key){
    int rv = -1;
    rwlock_acquire_readlock();
    node* curr = list->head;
    while (curr){
        if (curr->value == key){
            rv = 0;
            break;
        }
        curr = curr->next;
    }
    read_count++;
    printf("Read: %d\n", read_count);
    rwlock_release_readlock();
    return rv;
}

void free_node(node* nde){
    if (nde == NULL) return;
    free_node(nde->next);
    free(nde);
}

void free_list(ll** list){
    if (list == NULL || *list == NULL) return;
    free_node((*list)->head);
    free(*list);
    *list = NULL;
}

void* writing(void* arg){
    int thread_id = *((int*)arg);
    for (int i=0 ; i<1000 ; i++){
        int num = rand()%1000;
        insert(num);
        // printf("Thread %d wrote %d\n", thread_id , num);
    }
    return NULL;
}

void* reading(void* arg){
    int thread_id = *((int*)arg);
    for (int i=0 ; i<1000 ; i++){
        List_Lookup(-2);
        // printf("Thread %d read done\n" , thread_id);
    }
    return NULL;
}

int main(){
    rw = (rwlock_t*)malloc(sizeof(rwlock_t));
    rwlock_init();
    list = (ll*)malloc(sizeof(ll));
    init();


    pthread_t threads[160];
    int ids[160];

    for (int i=0 ; i<80 ; i++){
        ids[i] = i;
        pthread_create(&threads[i] , NULL , writing , &ids[i]);
    }
    for (int i=80 ; i<160 ; i++){
        ids[i] = i;
        pthread_create(&threads[i] , NULL , reading , &ids[i]);
    }

    for (int i=0 ; i<160 ; i++){
        pthread_join(threads[i] , NULL);
    }

    return 0;
}