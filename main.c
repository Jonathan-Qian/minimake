#include <stdio.h>

#include "defs.h"

int main() {
    BuildContext buildContext;
    buildContext.targets.arr = NULL;
    buildContext.targets.size = 0;

    parse(&(buildContext.targets));
    build_graph(&(buildContext.targets));


    for (int i = 0; i < buildContext.targets.size; i++) {
        print_target(buildContext.targets.arr[i]);
    }

    return 0;
}