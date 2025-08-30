#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "path.h"

int main() {
    struct path * p = path_new("testing");
    assert(p);
    assert(path_append(p, "first"));
    assert(path_append(p, "second"));
    assert(strcmp(path_value(p), "testing/first/second") == 0);
    assert(strcmp(path_remove(p), "testing/first") == 0);
    assert(strcmp(path_append(p, "third"), "testing/first/third") == 0);
    path_remove(p);
    assert(strcmp(path_remove(p), "testing") == 0);
    assert(!path_remove(p));
    assert(strcmp(path_value(p), "testing") == 0);
    assert(path_append(p, "first//"));
    assert(path_append(p, "second///"));
    assert(strcmp(path_value(p), "testing/first/second") == 0);
    assert(strcmp(path_remove(p), "testing/first") == 0);
    assert(strcmp(path_append(p, "third/"), "testing/first/third") == 0);
    path_remove(p);
    assert(strcmp(path_remove(p), "testing") == 0);
    assert(!path_remove(p));
    assert(strcmp(path_value(p), "testing") == 0);
    path_destroy(p);
    p = path_new("/bin/ls");
    assert(p);
    assert(strcmp(path_remove(p), "/bin") == 0);
    assert(strcmp(path_remove(p), "/") == 0);
    assert(!path_remove(p));
    path_destroy(p);
    p = path_new("bin//ls");
    assert(strcmp(path_value(p), "bin/ls") == 0);
    assert(strcmp(path_append(p, "///ciao//mamma/"), "bin/ls/ciao/mamma") == 0);
    path_destroy(p);

    printf("TEST PASSED.\n");
    return 0;
}