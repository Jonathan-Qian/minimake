#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "threadpool.h"
#include "defs.h"

void init_thread_pool(ThreadPool* pool, int num_threads) {
    pool->threads = (pthread_t*) calloc(num_threads, sizeof(pthread_t));
    pool->num_threads = num_threads;
    pool->queue.head = NULL;
    pool->queue.tail = NULL;
    pool->queue.num_tasks = 0;
    pthread_mutex_init(&(pool->queue.mutex), NULL);
    pthread_cond_init(&(pool->queue.cond), NULL);
}

void enqueue(TaskQueue* queue, Target* target) {
    target->flags = target->flags | TARGET_SCHEDULED;
    target->next = NULL;

    if (queue->tail) {
        queue->tail->next = target;
    }
    else {
        queue->head = target;
    }

    queue->tail = target;
    queue->num_tasks++;
}

Target* dequeue(TaskQueue* queue) {
    Target* t = queue->head;

    if (t) {
        queue->head = t->next;

        if (!queue->head) {
            queue->tail = NULL;
        }

        t->next = NULL;
        queue->num_tasks--;
    }

    return t;
}