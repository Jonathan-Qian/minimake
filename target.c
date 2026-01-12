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
    target->dependencies = NULL;
    target->num_dependencies = 0;
    target->num_target_dependencies = 0;
    target->dependents.arr = NULL;
    target->dependents.size = 0;
    target->commands = NULL;
    target->num_commands = 0;
    target->flags = TARGET_PENDING;
    target->next = NULL;
}

void print_target(Target* target) {
    printf("Name: %s\nDependencies (%d): ", target->name, target->num_dependencies);
    
    for (int i = 0; i < target->num_dependencies; i++) {
        printf("%s, ", target->dependencies[i]);
    }

    printf("\nCommands (%d):\n", target->num_commands);

    for (int i = 0; i < target->num_commands; i++) {
        printf("%s\n", target->commands[i]);
    }

    printf("Dependents (%d): ", target->dependents.size);

    for (int i = 0; i < target->dependents.size; i++) {
        printf("%s, ", target->dependents.arr[i]->name);
    }

    printf("\n\n");
}