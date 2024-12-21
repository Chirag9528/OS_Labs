#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct __Node{
    int value;
    struct __Node* next;
    pthread_mutex_t nlock;
}per_node_lock;


typedef struct __LinkedList{
    per_node_lock* head;
    pthread_mutex_t lock;
}ll_per_node_lock;

ll_per_node_lock* list;

void init_per_node_lock(ll_per_node_lock* list);

void print_per_node_lock(per_node_lock* head);

void insert_per_node_lock(ll_per_node_lock* list , int value);

void free_node(per_node_lock* nde);

void free_list(ll_per_node_lock** list);