#include <stdio.h>
#include <stdlib.h>

typedef struct cset {
    unsigned n;
    unsigned * U;
} cset;

/* complexity: O(n) */
cset * cset_new(unsigned int n) {
    cset * s = malloc(sizeof(cset));
    s->n = n;
    s->U = malloc(n * sizeof(unsigned));
    for (int i = 0; i < n; i++) {
        s->U[i] = 0;
    }
    return s;
}                                       

/* complexity: O(n) */
cset * cset_new_copy(const cset * s1) {
    cset * s2 = malloc(sizeof(cset));
    s2->n = s1->n;
    s2->U = malloc(s1->n * sizeof(unsigned));
    for (int i = 0; i < s1->n; i++) {
        s2->U[i] = s1->U[i];
    }
    return s2;
}                                  

/* complexity: O(1) */
void cset_destroy(cset * s) {
    free(s->U);
    free(s);
}                                           

/* complexity: O(1) */
void cset_insert(cset * s, unsigned int elem) {
    s->U[elem] = 1;
}                         

/* complexity: O(1) */
void cset_delete(cset * s, unsigned int elem) {
    s->U[elem] = 0;
}                         

/* complexity: O(1) */
int  cset_find(const cset * s, unsigned int elem) {
    return s->U[elem];
}                     

/* complexity: O(n) */
void cset_union(cset * a, const cset * b) {
    if (b->n > a->n) {
        a->U = realloc(a->U, b->n * sizeof(unsigned));
        for (int i = a->n; i < b->n; i++) {
            a->U[i] = 0;
        }
        a->n = b->n;
    }
    for (int i = 0; i < b->n; i++) {
        a->U[i] = (a->U[i] || b->U[i]) ? 1 : 0;
    }
}                             

/* complexity: O(n) */
void cset_intersection(cset * a, const cset * b) {
    if (b->n < a->n) {
        a->n = b->n;
        a->U = realloc(a->U, a->n * sizeof(unsigned));
    }
    for (int i = 0; i < a->n; i++) {
        a->U[i] = (a->U[i] && b->U[i]) ? 1 : 0;
    }
}                      

/* complexity: O(n) */
int  cset_is_empty(const cset * s) {
    for (int i = 0; i < s->n; i++) {
        if (s->U[i]) return 0;
    }
    return 1;
}                                    

/* complexity: O(n) */
int  cset_is_equal(const cset * a, const cset * b) {
    for (int i = 0; i < a->n && i < b->n; i++) {
        if (a->U[i] != b->U[i]) return 0;
    }
    if (a->n > b->n) {
        for (int i = b->n; i < a->n; i++) {
            if (a->U[i]) return 0;
        }
    }
    if (b->n > a->n) {
        for (int i = a->n; i < b->n; i++) {
            if (b->U[i]) return 0;
        } 
    }
    return 1;
}                    
