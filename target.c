#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

#include "defs.h"

static bool up_to_date(Target*, BuildContext*);

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
    target->num_remaining_targets = 0;
    pthread_mutex_init(&(target->num_mutex), NULL);
    target->flags = TARGET_PENDING;
    target->next = NULL;
}

int build_target(Target* target, int skip, BuildContext* b) {
    target->flags = target->flags | TARGET_BUILDING;
    
    bool u_t_d = true;

    if (skip == ERROR_TARGET_FAILED) {
        target->flags = target->flags | TARGET_SKIPPED;
        return ERROR_TARGET_FAILED;
    }
    else if (!up_to_date(target, b)) {
        u_t_d = false;

        char* command;
        pid_t pid;
        int status;

        for (int i = 0; i < target->num_commands; i++) {
            command = target->commands[i];

            pthread_mutex_lock(&(b->log_mutex));
            printf("%s\n", command);
            pthread_mutex_unlock(&(b->log_mutex));

            char *argv[MAX_ARGS];
            int i = 0;

            char *token = strtok(command, " ");

            while (token != NULL) {
                argv[i++] = token;
                token = strtok(NULL, " ");
            }

            argv[i] = NULL;

            pid = fork();

            if (pid == 0) {
                execvp(argv[0], argv);
            }

            waitpid(pid, &status, 0);

            if (WIFEXITED(status) == 0) {
                pthread_mutex_lock(&(b->log_mutex));
                fprintf(stderr, "Error: target %s failed building. The following command did not execute properly:\n%s\n", target->name, command);
                pthread_mutex_unlock(&(b->log_mutex));

                target->flags = target->flags | TARGET_FAILED;

                return ERROR_TARGET_FAILED;
            }
        }
    }

    target->flags = target->flags | TARGET_BUILT;
    
    return u_t_d ? 1 : 0;
}

bool up_to_date(Target* target, BuildContext* b) {
    struct stat target_stat, dep_stat;

    // checking if target exists
    if (stat(target->name, &target_stat) != 0) {
        return false;
    }

    for (int i = 0; i < target->num_dependencies_names; i++) {
        // checking if dependency exists
        if (stat(target->dependencies_names[i], &dep_stat) != 0) {
            pthread_mutex_lock(&(b->log_mutex));
            fprintf(stderr, "Error: target %s has file dependency %s that could not be accessed.\n", target->name, target->dependencies_names[i]);
            pthread_mutex_unlock(&(b->log_mutex));
            exit(1);
        }

        // checking if dependency is newer
        if (dep_stat.st_mtime > target_stat.st_mtime) {
            return false;
        }
    }

    return true;
}

// void print_target(Target* target) {
//     printf("Name: %s\nTarget Dependencies (%d): ", target->name, target->dependencies.size);
    
//     for (int i = 0; i < target->dependencies.size; i++) {
//         printf("%s, ", target->dependencies.arr[i]->name);
//     }

//     printf("\nDependencies (%d): ", target->num_dependencies_names);
    
//     for (int i = 0; i < target->num_dependencies_names; i++) {
//         printf("%s, ", target->dependencies_names[i]);
//     }

//     printf("\nCommands (%d):\n", target->num_commands);

//     for (int i = 0; i < target->num_commands; i++) {
//         printf("%s\n", target->commands[i]);
//     }

//     printf("Dependents (%d): ", target->dependents.size);

//     for (int i = 0; i < target->dependents.size; i++) {
//         printf("%s, ", target->dependents.arr[i]->name);
//     }

//     if ((target->flags & TARGET_VISITING) && !(target->flags & TARGET_VISITED)) {
//         printf("\n\n\n\nThe DFS traversal broke.\n\n\n\n");
//     }

//     printf("\nVisited: %s\n\n", (target->flags & TARGET_VISITED) ? "Yes" : "No");
// }