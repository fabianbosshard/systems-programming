#include <stdio.h>
#include <stdlib.h>

static int cmp_char(const void *a, const void *b) {
    return (*(char *)a - *(char *)b); // avoids overflow
}

/*
qsort needs a function that returns:
	•	< 0 if a < b
	•	0   if a == b
	•	> 0 if a > b
    */

int main(void) {
    char s[] = "12345542311";
    size_t n = sizeof(s) - 1;      // exclude '\0'
    qsort(s, n, sizeof s[0], cmp_char);
    printf("%.*s\n", (int)n, s);
}