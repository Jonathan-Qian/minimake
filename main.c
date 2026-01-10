#include "defs.h"

int main() {
    BuildContext buildContext;
    buildContext.targets.head = NULL;

    parse(&(buildContext.targets));

    return 0;
}