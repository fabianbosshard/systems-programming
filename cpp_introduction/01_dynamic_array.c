#include <stdio.h>
#include <stdlib.h>


/** BEGIN: dynamic array data structure */

struct vector {
    int * data;
    int capacity;
    int size;
};

struct vector * new_vector(int initial_capacity) {
    struct vector * vp = (struct vector *) malloc(sizeof(struct vector));

    if (initial_capacity < 1) initial_capacity = 1;
    vp->data = (int *) realloc(NULL, initial_capacity * sizeof(int));
    vp->capacity = initial_capacity;
    vp->size = 0;

    return vp;
}

void delete_vector(struct vector * vp) {
    free(vp->data);
    free(vp);
}

void push_back(struct vector * vp, int num) {
    if (vp->size == vp->capacity) {
        vp->capacity *= 2;
        vp->data = (int *) realloc(vp->data, vp->capacity * sizeof(int));
    }
    vp->data[vp->size] = num;
    vp->size++;
}

int pop_back(struct vector * vp) {
    int num;
    if (vp->size > 0) {
        vp->size--;
        num = vp->data[vp->size];
    } else exit(EXIT_FAILURE);
    if (vp->capacity > 1 && vp->size < vp->capacity / 2) {
        vp->capacity /= 2;
        vp->data = (int *) realloc(vp->data, vp->capacity * sizeof(int));
    }
    return num;
}

void sort_vector(struct vector * vp) {
    for (int i = 0; i < vp->size; i++) {
        for (int j = i + 1; j < vp->size; j++) {
            if (vp->data[j] < vp->data[i]) {
                int temp = vp->data[i];
                vp->data[i] = vp->data[j];
                vp->data[j] = temp;
            }
        }
    }
}

/** END: dynamic array data structure */



int main(int argc, char * argv[]) {
    struct vector * vp = new_vector(8);
    int x;
    while (scanf("%d", &x) == 1) {
        push_back(vp, x);
    }
    sort_vector(vp);
    printf("\n\n");
    for (int i = 0; i < vp->size; i++) {
        printf("%d\n", vp->data[i]);
    }
    delete_vector(vp);
}
