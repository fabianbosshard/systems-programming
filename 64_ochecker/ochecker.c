#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ochecker.h"


struct ochecker {
    oc_status_t state;

    char * data;
    size_t num_bytes;

    size_t i;
    size_t line;
    size_t column;
};

struct ochecker * oc_create () {
    struct ochecker * oc = malloc(sizeof(struct ochecker));
    oc->state = OC_CLOSED;
    oc->data = NULL;
    return oc;
}

void oc_destroy (struct ochecker * oc) {
    if (oc->data) free(oc->data);
    free(oc);
}

int oc_open_file (struct ochecker * oc, const char * fname) {
    FILE * fp = fopen(fname, "r");
    if (!fp) {oc->state = OC_CLOSED; return 0;}

    if (oc->data) free(oc->data);
    oc->data = malloc(sizeof(char));
    if (!oc->data) {oc->state = OC_CLOSED; return 0;}
    int c = 0;
    size_t i = 0;

    while ((c = getc(fp)) != EOF) {
        oc->data[i] = c;
        i++;
        oc->data = realloc(oc->data, (i + 1) * sizeof(char));
        if (!oc->data) {oc->state = OC_CLOSED; return 0;}
    }

    oc->data[i] = '\0';
    oc->num_bytes = i;
    fclose(fp);

    oc->state = OC_OPEN;
    oc->i = 0;
    oc->column = 0;
    oc->line = 0;
    return 1;
}

int oc_open_mem (struct ochecker * oc, const char * begin, const char * end) {
    if (end < begin) {oc->state = OC_CLOSED; return 0;}
    size_t i = end - begin;

    if (oc->data) free(oc->data);
    oc->data = malloc(sizeof(char) * (i + 1));
    if (!oc->data) {oc->state = OC_CLOSED; return 0;}

    memcpy(oc->data, begin, i);

    oc->data[i] = '\0';
    oc->num_bytes = i;

    oc->state = OC_OPEN;
    oc->i = 0;
    oc->column = 0;
    oc->line = 0;
    return 1;
}

int oc_open_str (struct ochecker * oc, const char * s) {
    size_t i = strlen(s);

    if (oc->data) free(oc->data);
    oc->data = malloc(sizeof(char) * (i + 1));
    if (!oc->data) {oc->state = OC_CLOSED; return 0;}

    memcpy(oc->data, s, i);

    oc->data[i] = '\0';
    oc->num_bytes = i;

    oc->state = OC_OPEN;
    oc->i = 0;
    oc->column = 0;
    oc->line = 0;
    return 1;
}

int oc_putc (struct ochecker * oc, char c) {
    if (oc->state != OC_OPEN) return 0;
    if (oc->i == oc->num_bytes || c != oc->data[oc->i]) {
        oc->state = OC_ERROR;
        if (oc->data) free(oc->data);
        oc->data = NULL;
        return 0;
    }
    oc->i++;
    if (c == '\n') {
        oc->column = 0;
        oc->line++;
    } else {
        oc->column++;
    }
    return 1;
}
int oc_puts (struct ochecker * oc, const char * s) {
    int status = 1;
    for (int j = 0; j < strlen(s); j++) {
        status = oc_putc(oc, s[j]);
        if (status == 0) return 0;
    }
    return 1;
}
int oc_write (struct ochecker * oc, const char * buf, size_t len) {
    int status = 1;
    for (int j = 0; j < len; j++) {
        status = oc_putc(oc, buf[j]);
        if (status == 0) return 0;
    }
    return 1;
}
int oc_close (struct ochecker * oc) {
    if (oc->state != OC_OPEN) return 0;
    if (oc->i != oc->num_bytes) {
        oc->state = OC_ERROR;
        if (oc->data) free(oc->data);
        oc->data = NULL;
        return 0;
    }
    oc->state = OC_CLOSED;
    if (oc->data) free(oc->data);
    oc->data = NULL;
    return 1;
}

oc_status_t oc_status (const struct ochecker * oc) {
    return oc->state;
}

size_t oc_position (const struct ochecker * oc) {
    return oc->i + 1;
}

size_t oc_line (const struct ochecker * oc) {
    return oc->line + 1;
}

size_t oc_column (const struct ochecker * oc) {
    return oc->column + 1;
}
