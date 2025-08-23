#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define N 101 //hashsize

struct listnode {
    struct listnode * next;
    char * n; // name
    char * d; // definition
};

struct listnode * T[N]; // hashtable

/* compute hash value from string (i.e. from a name) */
unsigned h(char * s) {
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++) {
        hashval = *s + 31 * hashval;
    }
    return hashval % N;
}

/* look for string in the hashtable (i.e. if the name already exists) */
struct listnode * lookup(char * s) {
    struct listnode * np;
    for (np = T[h(s)]; np != NULL; np = np->next) { // standard idiom for walking along a linked list
        if (strcmp(s, np->n) == 0) { 
            return np; // found name, so return address of it
        }
    }
    return NULL; // not found (i.e. name does not yet exist)
}

/* make a duplicate of string (by copying it into safe place) */
char * str_dub(char * str) {
    char * p;
    p = malloc(strlen(str) + 1); // +1 for '\0'
    strcpy(p, str);
    return p;
}

/* put the pair (name, definition) in T */
struct listnode * install(char * s, char * definition) {
    struct listnode * np;
    np = lookup(s);
    if (np == NULL) { // name not found in hashtable => create new node
        np = malloc(sizeof(struct listnode)); // create list node
        np->n = str_dub(s); // copy name
        np->next = T[h(s)];
        T[h(s)] = np; // insert at head
    } else { // name already there => free old definition
        free(np->d);
    }
    np->d = str_dub(definition); // copy definition
    return np;
}

/* remove given name from the hashtable */
void uninstall (char * s) {
    struct listnode * np = lookup(s);
    if (np == NULL) {
        return;
    }
    struct listnode * prev = T[h(s)];
    if (prev == np) {
        T[h(s)] = np->next;
    } else {
        while (prev->next != np) prev = prev->next; // obtain predecessor
        prev->next = np->next;
    }
    free(np->n);
    free(np->d);
    free(np);
}


int main(void) {
    char cmd[100], name[100], def[100];

    printf("Simple hashtable interface.\n");
    printf("Commands:\n");
    printf("  install <name> <definition>\n");
    printf("  lookup  <name>\n");
    printf("  uninstall <name>\n");
    printf("  print\n");
    printf("  quit\n");

    while (1) {
        printf("> ");
        if (scanf("%99s", cmd) != 1)
            break;

        if (strcmp(cmd, "install") == 0) {
            if (scanf("%99s %99s", name, def) == 2) {
                install(name, def);
                printf("Installed: %s = %s\n", name, def);
            } else {
                printf("Usage: install <name> <definition>\n");
            }

        } else if (strcmp(cmd, "lookup") == 0) {
            if (scanf("%99s", name) == 1) {
                struct listnode *np = lookup(name);
                if (np)
                    printf("%s = %s\n", np->n, np->d);
                else
                    printf("%s not found\n", name);
            }

        } else if (strcmp(cmd, "uninstall") == 0) {
            if (scanf("%99s", name) == 1) {
                uninstall(name);
                printf("Removed: %s\n", name);
            }

        } else if (strcmp(cmd, "print") == 0) {
            for (int i = 0; i < N; i++) {
                struct listnode *np = T[i];
                if (!np) continue;
                printf("[%d]:", i);
                for (; np; np = np->next)
                    printf(" (%s -> %s)", np->n, np->d);
                printf("\n");
            }

        } else if (strcmp(cmd, "quit") == 0) {
            break;

        } else {
            printf("Unknown command: %s\n", cmd);
        }
    }

    printf("Exiting.\n");
    return 0;
}
