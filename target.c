#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "defs.h"

void add_target(Target* target, TargetList* list) {
    list->arr = realloc(list->arr, (list->size + 1) * sizeof(Target*));
    list->arr[list->size++] = target;
}

void init_target(Target* target, const char* name) {
    target->name = strdup(name);
    target->dependencies_names = NULL;
    target->num_dependencies_names = 0;
    target->dependencies.arr = NULL;
    target->dependencies.size = 0;
    target->dependents.arr = NULL;
    target->dependents.size = 0;
    target->commands = NULL;
    target->num_commands = 0;
    target->flags = TARGET_PENDING;
    target->next = NULL;
}

void print_target(Target* target) {
    printf("Name: %s\nTarget Dependencies (%d): ", target->name, target->dependencies.size);
    
    for (int i = 0; i < target->dependencies.size; i++) {
        printf("%s, ", target->dependencies.arr[i]->name);
    }

    printf("\nDependencies (%d): ", target->num_dependencies_names);
    
    for (int i = 0; i < target->num_dependencies_names; i++) {
        printf("%s, ", target->dependencies_names[i]);
    }

    printf("\nCommands (%d):\n", target->num_commands);

    for (int i = 0; i < target->num_commands; i++) {
        printf("%s\n", target->commands[i]);
    }

    printf("Dependents (%d): ", target->dependents.size);

    for (int i = 0; i < target->dependents.size; i++) {
        printf("%s, ", target->dependents.arr[i]->name);
    }

    if ((target->flags & TARGET_VISITING) && !(target->flags & TARGET_VISITED)) {
        printf("\n\n\n\nThe DFS traversal broke.\n\n\n\n");
    }

    printf("\nVisited: %s\n\n", (target->flags & TARGET_VISITED) ? "Yes" : "No");
}