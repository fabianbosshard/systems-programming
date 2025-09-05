#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_LINE_OUTPUT 60



char * output_line(char * line, size_t min, size_t max) {
    if (strlen(line) < min || (strlen(line) > max && max != SIZE_T_MAX)) {
        return line;
    }
    if (strlen(line) > MAX_LINE_OUTPUT) {
        line = realloc(line, (MAX_LINE_OUTPUT + strlen("...") + 1) * sizeof(char)); // +1 for \0
        line[MAX_LINE_OUTPUT + 0] = '.';
        line[MAX_LINE_OUTPUT + 1] = '.';
        line[MAX_LINE_OUTPUT + 2] = '.';
        line[MAX_LINE_OUTPUT + 3] = '\0';
    }
    fputs(line, stdout);
    putc('\n', stdout);
    return line;
}


int main(int argc, char * argv[]) {
    size_t min = 0;
    size_t max = SIZE_T_MAX;

    if (argc == 2) {
        min = atoi(argv[1]);
    }
    if (argc == 3) {
        min = atoi(argv[1]);
        max = atoi(argv[2]);
    } 
    if (argc >= 4) {
        exit(EXIT_FAILURE);
    }

    char * line = malloc(sizeof(char));
    size_t i = 0;
    
    int c;
    while ((c = getc(stdin)) != EOF) {
        line = realloc(line, (i + 1) * sizeof(char));
        if (c == '\n') {
            line[i] = '\0';
            line = output_line(line, min, max);
            i = 0;
        } else {
            line[i] = c;
            i++;
        }
    }


    free(line);
}
