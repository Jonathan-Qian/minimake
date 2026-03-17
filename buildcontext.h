#ifndef BUILDCONTEXT_H
#define BUILDCONTEXT_H

#include "threadpool.h"

typedef struct BuildContext {
    TargetList targets;
    int argument_target_index;
    ThreadPool pool;
} BuildContext;

void init_build_context(BuildContext*);
void free_irrelevant_targets(BuildContext*);
void complete_tasks(BuildContext*);
void free_rest(BuildContext*);

#endif