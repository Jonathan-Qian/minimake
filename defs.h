#ifndef DEFS_H
#define DEFS_H

#define ERROR_IO 1
#define ERROR_TARGET 2
#define ERROR_INVALID 3
#define ERROR_DUPLICATE 4
#define ERROR_TARGET_NOT_FOUND 5

#define ERROR_TARGET_FAILED 2

#define MAX_STR 1024
#define MAX_ARGS 64

#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>

typedef enum Flag {
    TARGET_PENDING      = 1 << 0,
    TARGET_BUILDING     = 1 << 1,
    TARGET_BUILT        = 1 << 2,
    TARGET_FAILED       = 1 << 3,
    TARGET_SKIPPED      = 1 << 4,
    TARGET_SCHEDULED    = 1 << 5,
    TARGET_VISITING     = 1 << 6,
    TARGET_VISITED      = 1 << 7
} Flag;

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

typedef struct TaskQueue {
    Target* head;
    Target* tail;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} TaskQueue;

typedef struct ThreadPool {
    pthread_t* threads;
    int num_threads;
    TaskQueue queue;
    bool stop;
} ThreadPool;

typedef struct BuildContext {
    TargetList targets;
    int argument_target_index;
    ThreadPool pool;
    // pthread_mutex_t log_mutex;
} BuildContext;

int parse(BuildContext*, const char*);

void add_target(Target*, TargetList*);
void init_target(Target*, const char*);
int build_target(Target*, int, BuildContext*);

void build_graph(TargetList*);
int traverse(TaskQueue*, Target*);

void init_thread_pool(ThreadPool*, int);
void complete_tasks(BuildContext*);
void enqueue(TaskQueue*, Target*);
Target* dequeue(TaskQueue*);

void init_build_context(BuildContext*);
void free_irrelevant_targets(BuildContext*);
void free_rest(BuildContext*);

#endif