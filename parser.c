#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

#include "defs.h"

static void add_string(char***, int*, const char*);
static char* trim(char*);

void add_string(char*** list, int* n, const char* str) {
    *list = (char**) realloc(*list, (*n + 1) * sizeof(char*));
    (*list)[(*n)++] = strdup(str);
}

int parse(BuildContext* build_context, const char* argument_target) {
    FILE* f = fopen("Minimakefile", "r");

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
                fclose(f);
                return ERROR_TARGET;
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
                return ERROR_INVALID;
            }

            *colon = '\0';

            char* deps = colon + 1;

            trim(deps);
            trim(str);

            // duplicate target check
            for (int i = 0; i < build_context->targets.size; i++) {
                if (strcmp(str, build_context->targets.arr[i]->name) == 0) {
                    fprintf(stderr, "Error: target %s appears more than once.\n", str);
                    fclose(f);
                    return ERROR_DUPLICATE;
                }
            }

            current = (Target*) malloc(sizeof(Target));
            init_target(current, str);

            // add dependencies
            char* token = strtok(deps, " ");

            while (token) {
                trim(token);
                add_string(&(current->dependencies_names), &(current->num_dependencies_names), token);
                token = strtok(NULL, " ");
            }
            
            if (argument_target != NULL && strcmp(str, argument_target) == 0) {
                build_context->argument_target_index = build_context->targets.size;
            }

            add_target(current, &(build_context->targets));
        }
    }

    fclose(f);

    if (argument_target == NULL) {
        build_context->argument_target_index = 0;
    }
    else if (build_context->argument_target_index < 0) {
        fprintf(stderr, "Error: target %s not found.\n", argument_target);
        return ERROR_TARGET_NOT_FOUND;
    }

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