#include <pthread.h>
#include <stdlib.h>

#include "defs.h"

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