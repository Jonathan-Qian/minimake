#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "defs.h"

static void add_dependency(Target*, const char*);
static void add_target(Target*, TargetList*);
static char* trim(char*);

void add_dependency(Target* target, const char* dep) {
    target->deps = (char**) realloc(target->deps, (target->num_deps + 1) * sizeof(char*));
    target->deps[target->num_deps++] = strdup(dep);
}

void add_target(Target* target, TargetList* list) {
    target->next = list->head;
    list->head = target;
}

int parse(TargetList* targets) {
    FILE* f = fopen("Makefile", "r");

    if (f == NULL) {
        return ERROR_IO;
    }

    char line[MAX_STR], *str;
    Target* current = NULL;
    int counter = 0;

    while (fgets(line, sizeof(line), f) != NULL) {
        str = line;
        counter++;

        if (*str == '\t') {
            if (!current) {
                fprintf(stderr, "Error: Command without target.\n");
                fclose(f);
                exit(1);
            }

            trim(str);

            if (current->command != NULL) {
                free(current->command);
                fprintf(stderr, "Warning: Multiple commands for target %s. This is currently not supported. The last command associated with this target will be used.\n", current->name);
            }

            current->command = strdup(str);
        }
        else {
            trim(str);
            
            // ignore comments and empty lines
            if (*str == '#' || strlen(str) == 0) {
                continue;
            }

            char* colon = strchr(str, ':');

            if (!colon) {
                fprintf(stderr, "Error: line %d is invalid.\n", counter);
                fclose(f);
                exit(1);
            }

            *colon = '\0';

            char* deps = colon + 1;

            trim(deps);
            trim(str);

            current = (Target*) malloc(sizeof(Target));
            current->name = strdup(str);
            current->deps = NULL;
            current->num_deps = 0;
            current->command = NULL;
            current->flags = TARGET_PENDING;
            current->next = NULL;

            // add dependencies
            char* token = strtok(deps, " ");

            while (token) {
                trim(token);
                add_dependency(current, token);
                token = strtok(NULL, " ");
            }

            // for debugging
            printf("target:%s!  deps:", current->name);

            for (int i = 0; i < current->num_deps; i++) {
                printf("%s,", current->deps[i]);
            }

            printf("\n");
        }
    }

    fclose(f);

    return 0;
}

// trim() copied from Stack Overflow
char* trim(char* str) {
    size_t len = 0;
    char *frontp = str;
    char *endp = NULL;
    
    if (str == NULL) {
        return NULL;
    }

    if (str[0] == '\0') {
        return str;
    }
    
    len = strlen(str);
    endp = str + len;
    
    /* Move the front and back pointers to address the first non-whitespace
     * characters from each end.
     */
    while (isspace((unsigned char) *frontp)) {
        ++frontp;
    }

    if (endp != frontp) {
        while(isspace((unsigned char) *(--endp)) && endp != frontp) {}
    }
    
    if (frontp != str && endp == frontp) {
        // Empty string
        *(isspace((unsigned char) *endp) ? str : (endp + 1)) = '\0';
    }
    else if (str + len - 1 != endp) {
        *(endp + 1) = '\0';
    }
    
    /* Shift the string so that it starts at str so that if it's dynamically
     * allocated, we can still free it on the returned pointer.  Note the reuse
     * of endp to mean the front of the string buffer now.
     */
    endp = str;

    if (frontp != str) {
        while(*frontp) {
            *endp++ = *frontp++;
        }

        *endp = '\0';
    }
    
    return str;
}