#include <string.h>

#include "defs.h"

void resolve_dependencies(TargetList* list) {
    Target* target = list->head;
    char* dep;

    while(target) {
        for (int i = 0; i < target->num_deps; i++) {
            dep = target->deps[i];


        }

        target = target->next;
    }
}

int is_target(TargetList* list, char* name) {
    Target* target = list->head;

    while(target) {
        
        target = target->next;
    }
}