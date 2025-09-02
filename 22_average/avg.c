#include <stdio.h>
#include <stdlib.h>


float average_positives(const int * begin, const int * end) {
    unsigned N = 0;
    int s = 0;
    for (const int * p = begin; p != end; p++) {
        if (*p > 0) {
            s += *p;
            N++;
        }
    }
    if (N == 0) {
        return -1;
    }
    float avg = (float) s / (float) N;
    return avg;
}
