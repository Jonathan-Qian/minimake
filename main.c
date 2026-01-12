#include <stdio.h>
#include <stdlib.h>

#include "defs.h"

static int get_core_count();

int main(int argc, char *argv[]) {
    BuildContext buildContext;
    buildContext.targets.arr = NULL;
    buildContext.targets.size = 0;
    buildContext.argument_target_index = -1;

    int code = parse(&buildContext, argv[1]); // argv[1] == NULL if ran with no specified target

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

    build_graph(&(buildContext.targets));

    init_thread_pool(&(buildContext.pool), get_core_count());

    traverse(&(buildContext.pool.queue), buildContext.targets.arr[buildContext.argument_target_index]);

    // for debugging purposes
    for (int i = 0; i < buildContext.targets.size; i++) {
        print_target(buildContext.targets.arr[i]);
    }

    TaskQueue* q = &(buildContext.pool.queue);
    Target* t = q->head;

    while (t) {
        printf("%s -> ", t->name);
        t = t->next;
    }

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