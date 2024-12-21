#include <time.h>

typedef struct Node{
    int value;
    struct Node* next;
}node;


typedef struct LinkedList2{
    node* head;
}ll;

ll* list;

void init();

void print(node* head);

void insert(int value);

void free_node(node* nde);

void free_list(ll** list);