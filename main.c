#include <stdio.h>

#include "defs.h"

int main() {
    BuildContext buildContext;
    buildContext.targets.head = NULL;

    parse(&(buildContext.targets));

    // for debugging
    Target* current = buildContext.targets.head;
    
    while (current) {
        printf("%s\n", current->name);
        current = current->next;
    }

    return 0;
}