#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "defs.h"

static void* worker(void*);

void init_thread_pool(ThreadPool* pool, int num_threads) {
    pool->threads = (pthread_t*) calloc(num_threads, sizeof(pthread_t));
    pool->num_threads = num_threads;
    pool->queue.head = NULL;
    pool->queue.tail = NULL;
    pool->queue.num_tasks = 0;
    pthread_mutex_init(&(pool->queue.mutex), NULL);
    pthread_cond_init(&(pool->queue.cond), NULL);
}

void complete_tasks(BuildContext* context) {
    pthread_t* threads = context->pool.threads;

    for (int i = 0; i < context->pool.num_threads; i++) {
        pthread_create(threads + i, NULL, worker, context);
    }

    for (int i = 0; i < context->pool.num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
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

void* worker(void* b) {
    BuildContext* build_context = ((BuildContext*) b);
    TaskQueue* queue = &(build_context->pool.queue);
    int code = 0;
    Target* t;

    while (1) {
        pthread_mutex_lock(&(queue->mutex));

        while (!(t = dequeue(queue))) {
            pthread_cond_wait(&(queue->cond), &(queue->mutex));

            if (queue->num_tasks < 1) {
                pthread_mutex_unlock(&(queue->mutex));
                break;
            }
        }

        pthread_mutex_unlock(&(queue->mutex));

        code = build_target(t, code);

        free(t->name);

        for (int i = 0; i < t->num_dependencies_names; i++) {
            free(t->dependencies_names[i]);
        }

        free(t->dependencies_names);
        
        for (int i = 0; i < t->num_commands; i++) {
            free(t->commands[i]);
        }

        free(t->commands);

        if (t == build_context->targets.arr[build_context->argument_target_index]) {
            pthread_mutex_lock(&(queue->mutex));
            pthread_cond_broadcast(&(queue->cond));
            pthread_mutex_unlock(&(queue->mutex));

            free(t->dependents.arr);
            break;
        }
        else {
            Target* dependent;

            for (int i = 0; i < t->dependents.size; i++) {
                dependent = t->dependents.arr[i];

                pthread_mutex_lock(&(dependent->num_mutex));
                dependent->num_remaining_targets--;
                pthread_mutex_lock(&(queue->mutex));

                if (dependent->num_remaining_targets == 0 && (dependent->flags & TARGET_SCHEDULED) == 0) {
                    pthread_mutex_unlock(&(dependent->num_mutex));

                    enqueue(queue, dependent);
                    pthread_cond_signal(&(queue->cond));
                }
                else {
                    pthread_mutex_unlock(&(dependent->num_mutex));
                }

                pthread_mutex_unlock(&(queue->mutex));
            }

            free(t->dependents.arr);
        }
    }

    return NULL;
}