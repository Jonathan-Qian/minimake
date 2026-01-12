#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "defs.h"

static void add_string(char***, int*, const char*);
static char* trim(char*);

void add_string(char*** list, int* n, const char* str) {
    *list = (char**) realloc(*list, (*n + 1) * sizeof(char*));
    (*list)[(*n)++] = strdup(str);
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
            add_string(&(current->commands), &(current->num_commands), str);
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
            init_target(current, str);
            
            // add dependencies
            char* token = strtok(deps, " ");

            while (token) {
                trim(token);
                add_string(&(current->dependencies), &(current->num_dependencies), token);
                token = strtok(NULL, " ");
            }

            add_target(current, targets);
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