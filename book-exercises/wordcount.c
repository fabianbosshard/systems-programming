#include <stdio.h>

#define IN 1 // inside a word
#define OUT 0 // outside a word

int main() {
    int c, nl, nw, nc, state;

    state = OUT;
    nl = nw = nc = 0;

    while ((c = getchar()) != EOF) {
        if (c == ' ') {
            state = OUT;
        } else if (c == '\t') {
            state = OUT;
        } else if (c == '\n') {
            state = OUT;
            nl++;
        } else {
            if (state == OUT) {
                nw++;
            }
            state = IN;
        }
        nc++;
    }

    printf("\ncharacters: %d\n", nc);
    printf("words:      %d\n", nw);
    printf("lines:      %d\n", nl);
}