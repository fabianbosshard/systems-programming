#include <stdio.h>
#include <stdlib.h>

struct sequence {
    const char * begin;
    const char * end;
};

int is_separator(char c, struct sequence * S) {
    for (int j = 0; j < S->end - S->begin; j++) {
        if (c == S->begin[j]) {
            return 1;
        }
    }
    return 0;
}

struct iterator {
    struct sequence * T;
    struct sequence * S;

    char * cur_pos;
};

struct iterator * create_iterator() {
    struct iterator * i = malloc(sizeof(struct iterator));
    if (!i) return NULL;

    i->T = malloc(sizeof(struct sequence));
    char * text = malloc(1 * sizeof(char));
    text[0] = '\0';
    i->T->begin = text;
    i->T->end = text + 0;

    i->S = malloc(sizeof(struct sequence));
    char * separators = malloc(3 * sizeof(char));
    separators[0] = ',';
    separators[1] = ';';
    separators[2] = '\0';
    i->S->begin = separators;
    i->S->end = separators + 2;

    i->cur_pos = i->T->begin;
    return i;
}

void destroy_iterator(struct iterator * i) {
    free(i->T->begin);
    free(i->T);
    free(i->S->begin);
    free(i->S);
    free(i);
}

int set_text(struct iterator * i, const struct sequence * T) {
    free(i->T->begin);
    size_t n = (size_t)(T->end - T->begin);
    char * text = malloc((n + 1) * sizeof(char));
    for (int j = 0; j < n; j++) {
        text[j] = T->begin[j];
    }
    text[n] = '\0'; // safety pad
    i->T->begin = text;
    i->T->end = text + n; // range excludes the '\0'
    i->cur_pos = i->T->begin;
    return 1;
}

int set_separators(struct iterator * i, const struct sequence * S) {
    free(i->S->begin);
    size_t n = (size_t)(S->end - S->begin);
    char * separators = malloc((n + 1) * sizeof(char));
    for (int j = 0; j < n; j++) {
        separators[j] = S->begin[j];
    }
    separators[n] = '\0'; // safety pad
    i->S->begin = separators;
    i->S->end = separators + n; // range excludes the '\0'
    return 1;
}

int get_next(struct iterator * i, struct sequence * X) {
    while (i->cur_pos != i->T->end && is_separator(*(i->cur_pos), i->S)) {
        i->cur_pos++;
    }
    if (i->cur_pos == i->T->end) return 0;
    X->begin = i->cur_pos;
    while (i->cur_pos != i->T->end && !is_separator(*(i->cur_pos), i->S)) {
        i->cur_pos++;
    }
    X->end = i->cur_pos;
    return 1;
}
