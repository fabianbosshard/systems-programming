#include <stdio.h>
#include <stdlib.h>


char * readline(FILE * fp) {
    char * buf = NULL;
    size_t i = 0;
    int c;
    while (1) {
        c = getc(fp);
        if (c == EOF) {
            break;
        }
        if (c == '\n') {
            if (!buf) {
                buf = malloc(sizeof(char));
                buf[0] = '\0';
            }
            break;
        }
        buf = realloc(buf, (i + 2) * sizeof(char));
        buf[i++] = (char)c;
        buf[i] = '\0';
    }
    return buf;
}

int main(int argc, char * argv[]) {

}