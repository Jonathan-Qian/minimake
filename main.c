#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "defs.h"

static int get_core_count();

int main(int argc, char *argv[]) {
    clock_t begin = clock();
    BuildContext build_context;
    init_build_context(&build_context);    

    int code = parse(&build_context, argv[1]); // argv[1] == NULL if ran with no specified target

    if (code) {
        char* err = NULL;
        
        switch (code) {
            case ERROR_IO:
                err = "Makefile could not be opened.";
                break;
            case ERROR_TARGET:
                err = "command without target.\n";
                break;
        }

        if (err) {
            printf("Error: %s\n", err);
        }

        exit(1);
    }

    build_graph(&(build_context.targets));
    init_thread_pool(&(build_context.pool), 1);
    traverse(&(build_context.pool.queue), build_context.targets.arr[build_context.argument_target_index]);
    complete_tasks(&build_context);
    free_all(&build_context);

    clock_t end = clock();

    // Calculate time spent in seconds
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("Thread count: %d\n%f\n", build_context.pool.num_threads, time_spent);

    return 0;
}

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

int get_core_count() {
    long count = -1;

    // try sysconf() for Linux/Unix/macOS
#ifdef _SC_NPROCESSORS_ONLN
    count = sysconf(_SC_NPROCESSORS_ONLN);
#endif

    // try Windows-specific API if sysconf wasn't used/available
#ifdef _WIN32
    if (count <= 0) {
        SYSTEM_INFO sysinfo;
        GetSystemInfo(&sysinfo);
        count = sysinfo.dwNumberOfProcessors;
    }
#endif

    // final fallback: if all methods fail, default to 1
    if (count <= 0) {
        count = 4; 
    }

    return (int) count;
}