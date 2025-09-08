#include "lists.h"

struct list * concatenate_all(int count, struct list * lists[]) {

    struct list * l = 0;
    struct list ** last_p = &l;

    for (int i = 0; i < count; i++) {
        if (!lists[i]) continue;
        *last_p = lists[i];
        while (lists[i]->next != 0) {
            lists[i] = lists[i]->next;
        }
        last_p = &(lists[i]->next);
    }

    return l;
}

struct list * merge_sorted(struct list * a, struct list * b) {

    struct list * l = 0;
    struct list ** last_p = &l;

    while (a && b) {
        if (a->value < b->value) {
            *last_p = a;
            last_p = &(a->next);
            a = a->next;
        } else {
            *last_p = b;
            last_p = &(b->next);
            b = b->next;
        }
    }
    while (a) {
            *last_p = a;
            last_p = &(a->next);
            a = a->next;
    }
    while (b) {
            *last_p = b;
            last_p = &(b->next);
            b = b->next;
    }

    return l;
}
