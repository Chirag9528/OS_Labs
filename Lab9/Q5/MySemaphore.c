#include "MySemaphore.h"

void Zem_init(Zem_t* s , int value){
    s->value = value;
    pthread_cond_init(&s->cond , NULL);
    pthread_mutex_init(&s->lock, NULL);
}

void Zem_wait(Zem_t* s){
    pthread_mutex_lock(&s->lock);
    while (s->value <= 0){
        pthread_cond_wait(&s->cond , &s->lock);
    }
    s->value--;
    pthread_mutex_unlock(&s->lock);
}

void Zem_post(Zem_t* s){
    pthread_mutex_lock(&s->lock);
    s->value++;
    pthread_cond_signal(&s->cond);
    pthread_mutex_unlock(&s->lock);
}
