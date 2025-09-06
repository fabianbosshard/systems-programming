#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define linelen 1000


void push(char c, unsigned *ip, char * line, char * openclose) {
    int j;
    for (j = 0; j < strlen(openclose) / 2; j++) {
        if (c == openclose[2 * j + 1] && (*ip > 0) && line[*ip - 1] == openclose[2 * j]) {
            *ip -= 2;
            return;
        }
    }
    line[*ip] = c;
}

int main(int argc, char * argv[]) {

    char default_openclose[] = "()[]{}";
    char * openclose = default_openclose;

    if (argc >= 2) {
        openclose = argv[1];
    }

    char line[linelen + 1];
    unsigned i = 0;
    int c;
    while ((c = getc(stdin)) != EOF) {
        if (c == '\n') {
            line[i] = '\0';
            fprintf(stdout, "%s\n", line);
            i = 0;
        } else {
            push(c, &i, line, openclose);
            i++;
        }
    }
}
