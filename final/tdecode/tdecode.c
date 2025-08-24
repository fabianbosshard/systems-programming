#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXLINELENGTH 1000

char * code[128];


void init_code() {
    for (int i = 0; i < 128; i++) code[i] = NULL;
}

char * get_word() {
    int c;
    char * s = malloc(sizeof(char));
    int i = 0;
    while (isalpha((c = getc(stdin)))) {
        s[i] = c;
        s = realloc(s, (i + 1) * sizeof(char));
        i++;
    }
    s[i] = '\0';
    ungetc(c, stdin);
    return s;
}

void get_code() {
    int c;
    while ((c = getc(stdin)) != '\n') {
        code[c - 128] = get_word();
    }
}

void delete_code() {
    for (int i = 0; i < 128; i++) {
        free(code[i]);
        code[i] = NULL;
    }
}

void decode_text() {
    int c;
    while ((c = getc(stdin)) != EOF) {
        if (!(c < 128)) {
            if (code[c - 128]) {
                fputs(code[c - 128], stdout);
            }
        } else {
            putc(c, stdout);
        }
    }
}


int main (int argc, char * argv[]) {
    init_code();
    int c = getc(stdin);
    ungetc(c, stdin);
    if (!(c < 128)) {
        get_code();
    }
    decode_text();
}