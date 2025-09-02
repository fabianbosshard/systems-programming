#include <assert.h>
#include <stdio.h>

#include "avg.h"

int main() {
    int buf1[] = { 0, -1.0, 0, 2, -2, 0, 4 }; 

    assert(average_positives(buf1, buf1 + 3) == -1);
    assert(average_positives(buf1, buf1) == -1);
    assert(average_positives(buf1 + 3, buf1 + 3) == -1);
    assert(average_positives(buf1 + 2, buf1 + 3) == -1);
    assert(average_positives(buf1 + 2, buf1 + 4) == 2);
    assert(average_positives(buf1 + 2, buf1 + 7) == 3);
    assert(average_positives(buf1, buf1 + 4) == 2);
    assert(average_positives(buf1, buf1 + 3) == -1);
    assert(average_positives(buf1 + 4, buf1 + 4) == -1);
    assert(average_positives(buf1 + 3, buf1 + 4) == 2);
    assert(average_positives(buf1, buf1 + 4) == 2);
    assert(average_positives(buf1, buf1 + 7) == 3);

    printf("TESTS PASSED.\n");
}
