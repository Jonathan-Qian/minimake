#include <string.h>
#include <stdio.h>

#include "defs.h"

void build_graph(TargetList* targets) {
    Target *ti, *tj;

    for (int i = 0; i < targets->size; i++) {
        ti = targets->arr[i];

        for (int j = 0; j < targets->size; j++) {
            tj = targets->arr[j];

            for (int k = 0; k < tj->num_dependencies; k++) {
                if (strcmp(ti->name, tj->dependencies[k]) == 0) {
                    add_target(tj, &(ti->dependents));
                }
            }
        }
    }
}