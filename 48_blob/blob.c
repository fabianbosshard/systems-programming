#include "blob.h"

unsigned int count_char(const struct blob * b, char c) {
    unsigned N = 0;
    struct chunk_list * x = (&(b->sentinel))->next;
    while (x != &(b->sentinel)) {
        for (int i = 0; i < x->c->length; i++) {
            if (x->c->data[i] == c) N++;
        }
        x = x->next;
    }
    return N;
}

unsigned int copy_to_buffer(const struct blob * b, char * buf, unsigned int maxlen) {
    unsigned m = 0;
    struct chunk_list * x = (&(b->sentinel))->next;
    while (x != &(b->sentinel)) {
        for (int i = 0; i < x->c->length; i++) {
            if (m < maxlen) {
                buf[m] = x->c->data[i];
                m++;
            } else {
                return maxlen + 1;
            }
        }
        x = x->next;
    }
    return m;
}

unsigned int copy_to_buffer_reverse(const struct blob * b, char * buf, unsigned int maxlen) {
    unsigned m = 0;
    struct chunk_list * x = (&(b->sentinel))->prev;
    while (x != &(b->sentinel)) {
        for (int i = x->c->length - 1; i >= 0; i--) {
            if (m < maxlen) {
                buf[m] = x->c->data[i];
                m++;
            } else {
                return maxlen + 1;
            }
        }
        x = x->prev;
    }
    return m;
}
