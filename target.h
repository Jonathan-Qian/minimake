#ifndef TARGET_H
#define TARGET_H

#include <pthread.h>
#include <stdint.h>

typedef struct TargetList {
    struct Target** arr;
    int size;
} TargetList;

typedef struct Target {
    char* name;
    char** dependencies_names;
    int num_dependencies_names;
    TargetList dependencies; // freed during graph traversal
    TargetList dependents;
    char** commands;
    int num_commands;
    int num_remaining_targets; // should be the same as dependencies.size until tasks start executing
    pthread_mutex_t num_mutex;
    uint8_t flags;
    struct Target* next;
} Target;

void add_target(Target*, TargetList*);
void init_target(Target*, const char*);
int build_target(Target*, int);

#endif