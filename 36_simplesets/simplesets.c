#include "simplesets.h"
#include <stdio.h>
#include <stdlib.h>


/** BEGIN: doubly linked list */
struct List {
    struct ss * nil;
};
struct ID {
    char dummy;
};
struct ss { // list node (corresponds to one singleton x)
    struct ss * next;
    struct ss * prev;
    struct ID * id;
};
int list_init(struct List * L) {
    L->nil = (struct ss *) malloc(sizeof(struct ss));
    L->nil->next = L->nil;
    L->nil->prev = L->nil;
    return 1;
}
int list_insert(struct List * L, struct ss * x) {
    x->next = L->nil->next;
    L->nil->next->prev = x;
    L->nil->next = x;
    x->prev = L->nil;
    return 1;
}
void list_delete(struct ss * x) {
    x->prev->next = x->next;
    x->next->prev = x->prev;
    free(x);
}
struct ss * list_search(struct List * L, struct ID * id) {
    struct ss * x = L->nil->next;
    while (x != L->nil && x->id != id) {
        x = x->next;
    }
    return x;
}
void list_clear(struct List * L) { // deletes all elements except for the sentinel
    struct ss * x = L->nil->next;
    while (x != L->nil) {
        struct ss * todelete = x;
        x = x->next;
        struct ID * id_todelete = todelete->id;
        if (id_todelete) {
            struct ss * y = L->nil->next;
            while (y != L->nil) {
                struct ss * todelete = y;
                y = y->next;
                if (y->id == id_todelete) {
                    y->id = NULL;
                }
            }
            free(id_todelete);
        }
        list_delete(todelete);
    }
}
/** END: doubly linked list */



struct List L_ = {NULL};

/* Create a set containing a single object. */
struct ss * ss_create_singleton() {
    if (L_.nil == NULL) list_init(&L_);
    
    struct ss * Z = malloc(sizeof(struct ss));
    struct ID * id = malloc(sizeof(struct ID));
    Z->id = id;
    list_insert(&L_, Z);
    return Z;
}

/* Destroy all previously created sets. */
void ss_destroy_all() {
    list_clear(&L_);
}

/* Merge two sets into a single set.
 *
 * As a result, all the elements in X and Y belong to the same set,
 * and both X and Y now refer to the same set.
 */
void ss_merge(struct ss * X, struct ss * Y) {
    if (X->id == Y->id) return;

    struct ID * Y_id = Y->id;
    struct ss * Z;
    while ((Z = list_search(&L_, Y_id)) != L_.nil) {
        Z->id = X->id;
    }
    free(Y_id);
}

/* Test whether two sets are disjoint. */
int ss_disjoint(struct ss * X, struct ss * Y) {
    return (X->id != Y->id);
}
