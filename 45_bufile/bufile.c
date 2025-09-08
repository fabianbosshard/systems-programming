#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bufile.h"

struct bufile {
    char * begin;
    char * end;

    char * pos;
};

typedef struct bufile BUFILE;

BUFILE * bufopen(char * begin, char * end) {
    BUFILE * buf = malloc(sizeof(BUFILE));
    if (!buf) return NULL;

    buf->begin = begin;
    buf->end = end;

    buf->pos = begin;

    return buf;
}

void bufclose(BUFILE * buf) {
    free(buf);
}

char *bufgets(char *s, int size, BUFILE * buf) {
    int i = 0;
    if (buf->pos == buf->end) return NULL;

    while (buf->pos < buf->end && i < size - 1) {
        char c = *(buf->pos);
        s[i] = c;
        i++;
        buf->pos++;
        if (c == '\n') break;
    }
    s[i] = '\0';
    return s;
}
