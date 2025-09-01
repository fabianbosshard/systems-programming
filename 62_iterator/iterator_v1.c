#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct sequence {
    const char * begin;
    const char * end;
};

int is_separator(char c, char * separators) {
    for (int j = 0; j < strlen(separators); j++) {
        if (c == separators[j]) {
            return 1;
        }
    }
    return 0;
}

struct iterator {
    char * text;
    char * separators;

    char * cur_pos;
};

struct iterator * create_iterator() {
    struct iterator * i = malloc(sizeof(struct iterator));
    if (!i) return NULL;
    i->text = malloc(1 * sizeof(char));
    i->text[0] = '\0';
    i->separators = malloc(3 * sizeof(char));
    i->separators[0] = ',';
    i->separators[1] = ';';
    i->separators[2] = '\0';
    i->cur_pos = i->text;
    return i;
}

void destroy_iterator(struct iterator * i) {
    free(i->text);
    free(i->separators);
    free(i);
}

int set_text(struct iterator * i, const struct sequence * T) {
    i->text = realloc(i->text, (T->end - T->begin + 1) * sizeof(char));
    if (!i->text) return 0;
    for (int j = 0; j < T->end - T->begin; j++) {
        i->text[j] = T->begin[j];
    }
    i->text[T->end - T->begin] = '\0';
    i->cur_pos = i->text;
    return 1;
}

int set_separators(struct iterator * i, const struct sequence * S) {
    i->separators = realloc(i->separators, (S->end - S->begin + 1) * sizeof(char));
    if (!i->separators) return 0;
    for (int j = 0; j < S->end - S->begin; j++) {
        i->separators[j] = S->begin[j];
    }
    i->separators[S->end - S->begin] = '\0';
    return 1;
}

int get_next(struct iterator * i, struct sequence * X) {
    while (is_separator(*(i->cur_pos), i->separators)) {
        i->cur_pos++;
    }
    if (*(i->cur_pos) == '\0') return 0;
    X->begin = i->cur_pos;
    while (!is_separator(*(i->cur_pos), i->separators) && *(i->cur_pos) != '\0') {
        i->cur_pos++;
    }
    X->end = i->cur_pos;
    return 1;
}
