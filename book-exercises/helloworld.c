#include <stdio.h>

int main()
{
    int c;

    while ((c = getchar()) != EOF) {
        putchar(c);
    }

    // exercise 1-7 
    printf("value of EOF: %d\n", EOF);
}