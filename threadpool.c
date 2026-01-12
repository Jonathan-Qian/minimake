#include <pthread.h>
#include <stdlib.h>

#include "defs.h"

static void* worker(void*);

void init_thread_pool(ThreadPool* pool, int num_threads) {
    pool->threads = (pthread_t*) calloc(num_threads, sizeof(pthread_t));
    pool->num_threads = num_threads;
    pool->queue.head = NULL;
    pool->queue.tail = NULL;
    pthread_mutex_init(&(pool->queue.mutex), NULL);
    pthread_cond_init(&(pool->queue.cond), NULL);
}

void start_thread_pool(ThreadPool* pool) {
    pthread_t* threads = pool->threads;

    for (int i = 0; i < pool->num_threads; i++) {
        pthread_create(threads + i, NULL, worker, NULL);
    }
}

void enqueue(TaskQueue* queue, Target* target) {
    if (queue->tail) {
        queue->tail->next = target;
    }
    else { // in the case that the queue is empty
        queue->head = target;
    }

    queue->tail = target;
}

Target* dequeue(TaskQueue* queue) {
    Target* t = queue->head;

    if (t) {
        queue->head = t->next;
    }
    else if (!(queue->head)) {
        queue->tail = NULL;
    }

    return t;
}

void* worker(void* q) {
    TaskQueue* queue = ((TaskQueue*) q);

    while (1) {
        Target* t;
        pthread_mutex_lock(&(queue->mutex));

        if (!(t = dequeue(queue))) {
            pthread_cond_wait(&(queue->cond), &(queue->mutex));
            t = dequeue(queue);
        }

        pthread_mutex_unlock(&(queue->mutex));


    }
}