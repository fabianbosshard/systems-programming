#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


static struct stringstack {
    char * memory;
    size_t capacity;
    size_t top;
} S;

void strstack_use_buffer(char * mem, size_t mem_size) {
    S.memory = mem;
    S.capacity = mem_size;
    S.top = 0;
}

void strstack_clear() {
    S.top = 0;
}

int strstack_push(const char * s) {
    size_t n = strlen(s) + 1; // bytes to push (+1 for '\0')
    if (S.top +  n > S.capacity) {
        return 0;
    }
    memcpy(S.memory + S.top, s, n);
    S.top += n;
    return 1;
}
 
const char * strstack_pop() {
    if (S.top == 0) {
        return NULL;
    }
    size_t n = 1;
    while (S.top - n > 0 && *(S.memory + S.top - 1 - n) != '\0') {
        n++;
    }
    S.top -= n;
    return S.memory + S.top;
}