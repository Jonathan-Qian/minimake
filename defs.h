#ifndef DEFS_H
#define DEFS_H

#define ERROR_IO 1

#define MAX_STR 1024

#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>

typedef enum Flag {
    TARGET_PENDING  = 1 << 0,
    TARGET_BUILDING = 1 << 1,
    TARGET_BUILT    = 1 << 2,
    TARGET_FAILED   = 1 << 3
} Flag;

typedef struct TargetList {
    struct Target** arr;
    int size;
} TargetList;

typedef struct Target {
    char* name;
    char** dependencies;
    int num_dependencies;
    int num_target_dependencies;
    TargetList dependents;
    char** commands;
    int num_commands;
    uint8_t flags;
    struct Target* next;
} Target;

typedef struct Task {
    Target* target;
    struct Task* next;
} Task;

typedef struct TaskQueue {
    Task* head;
    Task* tail;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} TaskQueue;

typedef struct ThreadPool {
    pthread_t* threads;
    int thread_count;
    TaskQueue queue;
    bool running;
} ThreadPool;

typedef struct BuildContext {
    TargetList targets;
    ThreadPool pool;
    pthread_mutex_t graph_mutex;
    pthread_mutex_t log_mutex;
    sem_t job_limit;
} BuildContext;

int parse(TargetList*);

void add_target(Target*, TargetList*);
void init_target(Target*, const char*);
void print_target(Target*);

void build_graph(TargetList*);

#endif