#include <pthread.h>
#include <stdlib.h>

#include "defs.h"

void init_build_context(BuildContext* context) {
    context->targets.arr = NULL;
    context->targets.size = 0;
    context->argument_target_index = -1;
    pthread_mutex_init(&(context->log_mutex), NULL);
}

void free_all(BuildContext* context) {
    Target* t;

    for (int i = 0; i < context->targets.size; i++) {
        t = context->targets.arr[i];

        free(t->name);
        
        for (int j = 0; j < t->num_dependencies_names; j++) {
            free(t->dependencies_names[j]);
        }

        free(t->dependencies_names);
        free(t->dependencies.arr);
        free(t->dependents.arr);
        
        for (int j = 0; j < t->num_commands; j++) {
            free(t->commands[j]);
        }

        free(t->commands);
        pthread_mutex_destroy(&(t->num_mutex));
        free(t);
    }

    free(context->targets.arr);

    ThreadPool* pool = &(context->pool);
    free(pool->threads);
    pthread_mutex_destroy(&(pool->queue.mutex));
    pthread_cond_destroy(&(pool->queue.cond));

    pthread_mutex_destroy(&(context->log_mutex));
}