#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct path {
    char * pathname;
    int maxlen;
};

void path_normalize(char * start) {
    char * p = start;
    char * q = p;
    
    normal:
    if (*p != '/' && *p != '\0') {
        *(q++) = *(p++);
        goto normal;
    }
    if (*p == '/') {
        *(q++) = *(p++);
        goto slash;
    }
    if (*p == '\0') {
        *q = '\0';
        return;
    }

    slash:
    if (*p == '/') {
        p++;
        goto slash;
    }
    if (*p != '/' && *p != '\0') {
        *(q++) = *(p++);
        goto normal;
    }
    if (*p == '\0') {
        if (q - start > 1) q--;
        *q = '\0';
        return;
    }
}

struct path * path_new(const char * name) {
    struct path * path = malloc(sizeof(struct path));
    path->maxlen = PATH_MAX;
    path->pathname = malloc(PATH_MAX * sizeof(char));
    char * q = path->pathname;
    strcpy(q, name);
    path_normalize(path->pathname);
    return path;
}

void path_destroy(struct path * path) {
    free(path->pathname);
    free(path);
}

const char * path_append(struct path * this, const char * name) {
    char * q = this->pathname + strlen(this->pathname);
    *(q++) = '/';
    strcpy(q, name);
    path_normalize(this->pathname);
    return this->pathname;
}

int can_remove(struct path * path) {
    char * p = path->pathname;
    if (*p == '/') return (strlen(p) > 1) ? 1 : 0;
    while (*p != '/' && *p != '\0') p++;
    return (*p == '\0') ? 0 : 1;
}

const char * path_remove(struct path * this) {
    if (!can_remove(this)) return NULL;
    char * q = this->pathname + strlen(this->pathname) - 1;
    while (*q != '/') {
        *q = '\0';
        q--;
    }
    if (strlen(this->pathname) > 1) *q = '\0';  
    return this->pathname;
}

const char * path_value(const struct path * this) {
    return this->pathname;
}
