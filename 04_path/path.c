#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


struct path {
    char * pathname;
    int maxlen;
};


struct path * path_new(const char * name) {
    struct path * path = malloc(sizeof(struct path));
    path->maxlen = PATH_MAX;
    path->pathname = malloc(PATH_MAX * sizeof(char));

    char * q = path->pathname;
    char * p = name;
    
    normal:
    if (*p != '/' && *p != '\0') {
        *q = *p;
        q++, p++;
        goto normal;
    }
    if (*p == '/') {
        *q = *p;
        q++, p++;
        goto slash;
    }
    if (*p == '\0') {
        *q = '\0';
        goto end;
    }

    slash:
    if (*p == '/') {
        p++;
        goto slash;
    }
    if (*p != '/' && *p != '\0') {
        *q = *p;
        q++, p++;
        goto normal;
    }
    if (*p == '\0') {
        q--;
        *q = '\0';
        goto end;
    }

    end:

    return path;
}

void path_destroy(struct path * path) {
    free(path->pathname);
    free(path);
}

const char * path_append(struct path * this, const char * name) {
    char * q = this->pathname + strlen(this->pathname);
    char * p = name;
    *q = '/';
    q++;

    slash:
    if (*p == '/') {
        p++;
        goto slash;
    }
    if (*p != '/' && *p != '\0') {
        *q = *p;
        q++, p++;
        goto normal;
    }
    if (*p == '\0') {
        q--;
        *q = '\0';
        goto end;
    }

    normal:
    if (*p != '/' && *p != '\0') {
        *q = *p;
        q++, p++;
        goto normal;
    }
    if (*p == '/') {
        *q = *p;
        q++, p++;
        goto slash;
    }
    if (*p == '\0') {
        *q = '\0';
        goto end;
    }

    end:
    return this->pathname;
}

int can_remove(struct path * path) {
    if (*(path->pathname) == '/') {
        if (strlen(path->pathname) > 1) {
            return 1;
        } else {
            return 0;
        }
    }
    char * p = path->pathname;
    while (*p != '/' && *p != '\0') {
        p++;
    }
    if (*p == '\0') {
        return 0;
    }
    return 1;
}
const char * path_remove(struct path * this) {
    if (!can_remove(this)) return NULL;
    char * q = this->pathname + strlen(this->pathname) - 1;
    while (*q != '/') {
        *q = '\0';
        q--;
    }
    if (strlen(this->pathname) > 1) {
        *q = '\0';  
    }
    return this->pathname;
}
const char * path_value(const struct path * this) {
    return this->pathname;
}
