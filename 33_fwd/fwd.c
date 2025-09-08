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
    if (sscanf(prefix, "%hhu.%hhu.%hhu.%hhu/%zu", &p[0], &p[1], &p[2], &p[3], &l) != 5) return 0; // %d stores into int (4 bytes). p[j] is 1 byte and l is size_t => wrong sizes, trashes memory => Use %hhu and %zu

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
    if (l == 0) return 1; // `0xFFFFFFFF << 32` is UB: “If the value of the right operand is negative or is greater than or equal to the width of the promoted left operand, the behavior is undefined.” (https://web.cs.dal.ca/~vlado/pl/C_Standard_2011-n1570.pdf)
    u_int32_t mask = (0xFFFFFFFF << (32 - l));
    return (x & mask) == (p & mask);
}

int fwd_forward(const struct fwd_table * t, const char * address) {
    int port = -1;

    u_int8_t a[4];
    if (sscanf(address, "%hhu.%hhu.%hhu.%hhu", &a[0], &a[1], &a[2], &a[3]) != 4) return 0; 

    u_int32_t x = 0;
    for (int j = 0; j < 4; j++) {
        x |= (a[j] << ((3 - j) * 8));
    }

    size_t max = 0;
    for (int k = 0; k < t->n; k++) {
        size_t l = t->T[k].pref.l;
        if (l >= max && match(t->T[k].pref, x)) { // since in tests, tie winner has to be the newest (solution uses `>` + traversal newest -> oldest), we have to use `>=`, since we are iterating oldest -> newest
            max = l;
            port = t->T[k].port;
        }
    }
    
    return port;
}
