#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


struct server {
    struct inter * nil;
};

struct receiver {
    void (*deliver)(struct receiver * r, const char * message); // this does not declare a function, but a pointer (to a function)!
};


/** BEGIN: doubly linked list */
struct inter { // list node (corresponds to one interest)
    char * tag;
    struct receiver * r; // which receiver is interested in the tag?
    struct inter * next;
    struct inter * prev;
};
int list_init(struct server * L) {
    L->nil = (struct inter *) malloc(sizeof(struct inter));
    L->nil->next = L->nil;
    L->nil->prev = L->nil;
    L->nil->r = NULL;
    L->nil->tag = NULL;
    return 1;
}
int list_insert(struct server * L, struct inter * x) {
    x->next = L->nil->next;
    L->nil->next->prev = x;
    L->nil->next = x;
    x->prev = L->nil;
    return 1;
}
void list_delete(struct inter * x) {
    x->prev->next = x->next;
    x->next->prev = x->prev;
    free(x->tag);
    free(x);
}
struct inter * list_search(struct server * L, char * key) {
    struct inter * x = L->nil->next;
    while (x != L->nil && strcmp(key, x->tag) != 0) {
        x = x->next;
    }
    return x;
}
void list_clear(struct server * L) { // deletes all elements except for the sentinel
    struct inter * x = L->nil->next;
    while (x != L->nil) {
        struct inter * todelete = x;
        x = x->next;
        list_delete(todelete);
    }
}
/** END: doubly linked list */


struct server * server_new(){ /* return 0 on failure */
    struct server * s = (struct server *) malloc(sizeof(struct server));
    list_init(s);
    return s;
}

void server_delete(struct server * s) {
    list_clear(s); // clear interest list (deleting all elements except sentinel)
    free(s->nil); // delete sentinel
    free(s); // delete server
}

int add_interest(struct server * srv, struct receiver * r, const char * interest) {
    
    // create and initialize interest object x
    struct inter * x = (struct inter *) malloc(sizeof(struct inter));
    x->r = r;
    unsigned n = strlen(interest);
    x->tag = (char *) malloc(sizeof(char) * (n + 1));
    memcpy(x->tag, interest, n);
    x->tag[n] = '\0';

    // insert interest object x into linked list of server srv
    list_insert(srv, x);

    return 1;
}

void remove_interest(struct server * srv, struct receiver * r, const char * interest) {
    struct inter * x = srv->nil->next;
    while (x != srv->nil) {
        struct inter * next = x->next;
        if (x->r == r && strcmp(interest, x->tag) == 0) {
            struct inter * todelete = x;
            list_delete(todelete);
        }
        x = next;
    }
}

void clear_receiver(struct server * srv, struct receiver * r) {
    struct inter * x = srv->nil->next;
    while (x != srv->nil) {
        struct inter * next = x->next;
        if (x->r == r) {
            struct inter * todelete = x;
            list_delete(todelete);
        }
        x = next;
    }
}


int already_delivered(struct receiver * r, struct receiver * * list_delivered, unsigned num_delivered) {
    for (int i = 0; i < num_delivered; i++) {
        if (r == list_delivered[i]) {
            return 1;
        }
    }
    return 0;
}

void send_message_with_tag(const struct server * srv, const char * message, char * tag, struct receiver * * * Dp, unsigned * Np) {
    struct inter * x = srv->nil->next;
    while (x != srv->nil) {
        if (strcmp(x->tag, tag) == 0 && !already_delivered(x->r, *Dp, *Np)) {
            x->r->deliver(x->r, message);
            (*Np)++;
            (*Dp) = (struct receiver * *) realloc((*Dp), (*Np) * sizeof(struct receiver *));
            (*Dp)[(*Np) - 1] = x->r;
        }
        x = x->next;
    }
}

void clear_all(struct server * srv) {
    list_clear(srv);
}

int is_start_of_tag(const char * p) {
    if ((*p == '@' || *p == '#') && isalpha(*(p + 1))) {
        return 1;
    }
    if (isalpha(*p)) {
        return 1;
    }
    return 0;
}

const char * get_end_of_tag(const char * p) {
    while (isalpha(*p)) {
        p++;
    }
    return p;
}

void send(const struct server * srv, const char * message) {
    const char * p = message;

    struct receiver * * D = NULL; // list of already delivered receivers
    unsigned N = 0; // number of already delivered receivers

    while (*p != '\0') {
        if (is_start_of_tag(p)) {
            const char * end = get_end_of_tag(p + 1);
            unsigned n = end - p; // num of character
            char * tag = (char *) malloc(sizeof(char) * (n + 1)); // +1 for '\0'
            memcpy(tag, p, n);
            tag[n] = '\0';
            send_message_with_tag(srv, message, tag, &D, &N);
            free(tag);
            p = end;
        } else {
            p++;
        }
    }

    free(D);
};
