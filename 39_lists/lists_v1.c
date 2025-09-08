#include "lists.h"

struct list * concatenate_all(int count, struct list * lists[]) {

    struct list * begin = 0;
    struct list * to_be_connected = 0;

    for (int i = 0; i < count; i++) {
        if (!lists[i]) continue;
        if (!begin) {
            begin = lists[i];
        }
        if (to_be_connected) {
            to_be_connected->next = lists[i];
        }
        to_be_connected = lists[i];
        while (to_be_connected->next != 0) {
            to_be_connected = to_be_connected->next;
        }
    }

    return begin;
}

struct list * merge_sorted(struct list * a, struct list * b) {
    if (!a && !b) return 0;
    if (!a) return b;
    if (!b) return a;

    struct list * begin = 0;
    struct list * to_be_connected = 0;
    struct list * cur;
    while (a && b) {
        if (a->value < b->value) {
            cur = a;
            a = a->next;
        } else {
            cur = b;
            b = b->next;
        }
        if (!begin) {
            begin = cur;
        }
        if (to_be_connected) {
            to_be_connected->next = cur;
        }
        to_be_connected = cur;
    }
    while (a) {
        to_be_connected->next = a;
        to_be_connected = a;
        a = a->next;
    }
    while (b) {
        to_be_connected->next = b;
        to_be_connected = b;
        b = b->next;
    }

    return begin;

}
