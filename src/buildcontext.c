#include <pthread.h>
#include <stdlib.h>

#include "buildcontext.h"
#include "defs.h"

static void* worker(void*);

void init_build_context(BuildContext* context) {
    context->targets.arr = NULL;
    context->targets.size = 0;
    context->argument_target_index = -1;
}

// only use this function after traverse() has been called
void free_irrelevant_targets(BuildContext* context) {
    Target* t;

    for (int i = 0; i < context->targets.size; i++) {
        t = context->targets.arr[i];

        if ((t->flags & TARGET_VISITED) == 0) {
            free(t->name);

            for (int i = 0; i < t->num_dependencies_names; i++) {
                free(t->dependencies_names[i]);
            }

            free(t->dependencies_names);

            // dependencies.arr should have already been freed by traverse
            free(t->dependents.arr);
            
            for (int i = 0; i < t->num_commands; i++) {
                free(t->commands[i]);
            }

            free(t->commands);

            pthread_mutex_destroy(&(t->num_mutex));
            free(t);

            context->targets.arr[i] = NULL;
        }
    }
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

void free_rest(BuildContext* context) {
    ThreadPool* pool = &(context->pool);
    free(pool->threads);
    pthread_mutex_destroy(&(pool->queue.mutex));
    pthread_cond_destroy(&(pool->queue.cond));

    Target* t;

    for (int i = 0; i < context->targets.size; i++) {
        t = context->targets.arr[i];

        if (t) {
            pthread_mutex_destroy(&(t->num_mutex));
            free(t);
        }
    }

    free(context->targets.arr);
}