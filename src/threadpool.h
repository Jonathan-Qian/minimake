#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "target.h"

#include <pthread.h>

typedef struct TaskQueue {
    Target* head;
    Target* tail;
    int num_tasks;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} TaskQueue;

typedef struct ThreadPool {
    pthread_t* threads;
    int num_threads;
    TaskQueue queue;
} ThreadPool;

void init_thread_pool(ThreadPool*, int);
void enqueue(TaskQueue*, Target*);
Target* dequeue(TaskQueue*);

#endif