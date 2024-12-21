#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct Node{
    int value;
    struct Node* next;
}node;


typedef struct LinkedList2{
    node* head;
}ll;

ll* list;

void init(ll* list);

void print(node* head);

void insert(ll* list , int value);

void free_node(node* nde);

void free_list(ll** list);

int delete_node(ll* list , int value);