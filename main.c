#include <stdio.h>
#include <stdlib.h>

#include "defs.h"

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

    code = dfs(buildContext.targets.arr[buildContext.argument_target_index]);

    if (code != 0) {
        printf("\n\n\n\nError: circular dependency detected.\n\n\n\n");
        exit(1);
    }

    for (int i = 0; i < buildContext.targets.size; i++) {
        print_target(buildContext.targets.arr[i]);
    }

    return 0;
}