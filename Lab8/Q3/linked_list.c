#include "linked_list.h"

void init(ll* list){
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

void insert(ll* list , int value){
    node* new_node = (node*)malloc(sizeof(node));
    if (new_node == NULL){
        perror("malloc");
        return; 
    }
    new_node->value = value;
    new_node->next = list->head;
    list->head = new_node;
    return;
}

int delete_node(ll* list , int value){
    if (list == NULL || list->head == NULL) return -1;
    node* curr = list->head;
    while (curr->next && curr->next->value != value){
        curr = curr->next;
    }
    if (curr->next == NULL){
        return -1;
    }
    else{
        node* temp = curr->next;
        curr->next = curr->next->next;
        free(temp);
    }
}

int List_Lookup(ll* list , int key){
    int rv = -1;
    node* curr = list->head;
    while (curr){
        if (curr->value == key){
            rv = 0;
            break;
        }
        curr = curr->next;
    }
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

int main(){
    FILE* fp = fopen("data1.csv" , "w");
    if (fp == NULL){
        fprintf(stderr, "File opening failed.\n");
        exit(1);
    }
    fprintf(fp , "x,y\n");
    for (int i=1; i<=16384 ; i+=100){

        list = (ll*)malloc(sizeof(ll));
        init(list);
        
        clock_t start = clock();
        for (int j = 0 ; j< i ; j++){
            int num = rand()%2;
            if (num){
                List_Lookup(list , -2);
            }
            else{
                insert(list , j);
            }
        }
        
        clock_t end = clock();
        
        free_list(&list);
        double time = (double)(end-start);
        fprintf(fp , "%d,%f\n" , i , time);
    }
    return 0;
}