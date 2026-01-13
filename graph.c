#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "defs.h"

void build_graph(TargetList* targets) {
    Target *ti, *tj;

    for (int i = 0; i < targets->size; i++) {
        ti = targets->arr[i];

        for (int j = 0; j < targets->size; j++) {
            tj = targets->arr[j];

            for (int k = 0; k < tj->num_dependencies_names; k++) {
                if (strcmp(ti->name, tj->dependencies_names[k]) == 0) {
                    add_target(tj, &(ti->dependents));
                    add_target(ti, &(tj->dependencies));
                    tj->num_remaining_targets++;
                }
            }
        }
    }
}

// traverses the graph starting from target to mark all relevant dependencies, detect any cycles and enqueue the first runnable targets
int traverse(TaskQueue* q, Target* target) {
    target->flags = target->flags | TARGET_VISITING;

    if (target->num_remaining_targets == 0) {
        enqueue(q, target);
    }
    else {
        for (int i = 0; i < target->dependencies.size; i++) {
            if ((target->dependencies.arr[i]->flags & TARGET_VISITED) == TARGET_VISITED) {
                continue;
            }
            else if ((target->dependencies.arr[i]->flags & TARGET_VISITING) == TARGET_VISITING) {
                fprintf(stderr, "Error: circular dependency detected.");
                exit(1);
            }
            
            // if the target is not visited or being visited, then it is unvisited
            traverse(q, target->dependencies.arr[i]);
        }
    }

    target->flags = target->flags | TARGET_VISITED;

    return 0;
}