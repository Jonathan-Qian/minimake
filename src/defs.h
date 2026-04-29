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

#endif