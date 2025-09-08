#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#include "fwd.h"

struct prefix {
    u_int32_t p;
    size_t l;
};

struct entry {
    struct prefix pref;
    int port;
};

struct fwd_table {
    struct entry * T;
    unsigned n;
};

struct fwd_table * fwd_new() {
    struct fwd_table * t = malloc(sizeof(struct fwd_table));
    t->T = NULL;
    t->n = 0;
    return t;
}

void fwd_delete(struct fwd_table * t) {
    fwd_clear(t);
    free(t);
}

int fwd_add(struct fwd_table * t, const char * prefix, int port) {
    u_int8_t p[4];
    size_t l;
    if (sscanf(prefix, "%d.%d.%d.%d/%d", &p[0], &p[1], &p[2], &p[3], &l) != 5) return 0;

    struct prefix pref = {0, 0};
    for (int j = 0; j < 4; j++) {
        pref.p |= (p[j] << ((3 - j) * 8));
    }
    pref.l = l;
    struct entry e = {pref, port};

    unsigned i = t->n;
    t->n++;
    t->T = realloc(t->T, t->n * sizeof(struct entry));
    t->T[i] = e;

    return 1;
}

void fwd_clear(struct fwd_table * t) {
    free(t->T);
    t->T = NULL;
    t->n = 0;
}

int match(struct prefix pref, u_int32_t x) {
    u_int32_t p = pref.p;
    int l = pref.l;
    u_int32_t mask = (0xFFFFFFFF << (32 - l));
    return (x & mask) == (p & mask);
}

int fwd_forward(const struct fwd_table * t, const char * address) {
    int port = -1;

    u_int8_t a[4];
    if (sscanf(address, "%d.%d.%d.%d", &a[0], &a[1], &a[2], &a[3]) != 4) return 0;

    u_int32_t x = 0;
    for (int j = 0; j < 4; j++) {
        x |= (a[j] << ((3 - j) * 8));
    }

    size_t max = 0;
    for (int k = 0; k < t->n; k++) {
        size_t l = t->T[k].pref.l;
        if (l > max && match(t->T[k].pref, x)) {
            max = l;
            port = t->T[k].port;
        }
    }
    
    return port;
}
