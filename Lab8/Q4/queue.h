#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct __node{
    int value;
    struct __node* next;
}node;

typedef struct __queue{
    node* head;
    node* tail;
}queue;

queue* q;

void init();

void enqueue(int value);

int dequeue(int* value);