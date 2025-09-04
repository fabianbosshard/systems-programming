#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void count_chars(FILE * input, const unsigned char * target_chars) {
    fflush(stderr);
    unsigned N = strlen(target_chars);
    unsigned * count = malloc(N * sizeof(unsigned));
    for (unsigned i = 0; i < N; i++) {
        count[i] = 0;
    }
    int c;
    while ((c = getc(input)) != EOF) {
        for (unsigned i = 0; i < N; i++) {
            if ((const unsigned char)c ==  (const unsigned char)target_chars[i]) {
                count[i]++;
            }
        }
    }
    for (unsigned i = 0; i < N; i++) {
        fprintf(stdout, " %c:%d", (char)target_chars[i], count[i]);
    }
    fprintf(stdout, "\n");
    free(count);
}

int main(int argc, char * argv[]) {
    if (argc == 1 || strlen(argv[1]) == 0) {
        fflush(stdout);
        fprintf(stderr, "No target characters specified.\n");
        exit(EXIT_FAILURE);
    }
    const unsigned char * target_chars = (const unsigned char *) argv[1];
    if (argc == 2) {
        fflush(stderr);
        fprintf(stdout, "(stdin)");
        count_chars(stdin, target_chars);
    } else {
        for (int i = 2; i < argc; i++) {
            FILE * fp = fopen(argv[i], "r");
            if (fp) {
                fflush(stderr);
                fprintf(stdout, "%s", argv[i]);
                count_chars(fp, target_chars);
            } else {
                fflush(stdout);
                fprintf(stderr, "Error reading file %s\n", argv[i]);
            }
        }
    }
    return EXIT_SUCCESS;
}
