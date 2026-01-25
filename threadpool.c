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
    pthread_mutex_init(&(pool->queue.mutex), NULL);
    pthread_cond_init(&(pool->queue.cond), NULL);
    pool->stop = false;
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
    else { // in the case that the queue is empty
        queue->head = target;
    }

    queue->tail = target;
}

Target* dequeue(TaskQueue* queue) {
    Target* t = queue->head;

    if (t) {
        queue->head = t->next;

        if (!queue->head) {
            queue->tail = NULL;
        }

        t->next = NULL;
    }
    else {
        queue->tail = NULL;
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

        while ((t = dequeue(queue)) == NULL && !build_context->pool.stop) {
            pthread_cond_wait(&(queue->cond), &(queue->mutex));
        }

        pthread_mutex_unlock(&(queue->mutex));

        if (build_context->pool.stop) {
            break;
        }

        code = build_target(t, code, build_context);

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
            build_context->pool.stop = true;
            pthread_cond_broadcast(&(queue->cond));
            pthread_mutex_unlock(&(queue->mutex));

            free(t->dependents.arr);
            break;
        }
        else {
            Target* d;

            for (int i = 0; i < t->dependents.size; i++) {
                d = t->dependents.arr[i];

                pthread_mutex_lock(&(d->num_mutex));
                d->num_remaining_targets--;
                pthread_mutex_lock(&(queue->mutex));

                if (d->num_remaining_targets == 0 && (d->flags & TARGET_SCHEDULED) == 0) {
                    pthread_mutex_unlock(&(d->num_mutex));

                    enqueue(queue, d);
                    pthread_cond_signal(&(queue->cond));
                    pthread_mutex_unlock(&(queue->mutex));
                }
                else {
                    pthread_mutex_unlock(&(d->num_mutex));
                    pthread_mutex_unlock(&(queue->mutex));
                }
            }

            free (t->dependents.arr);
        }
    }

    return NULL;
}