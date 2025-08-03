#include <stdio.h>
#include <stdlib.h>


char * readline(FILE * fp) {
    char * buf = NULL;
    size_t i = 0;
    int c;
    while (1) {
        c = getc(fp);
        if (c == EOF || c == '\n') break;
        buf = realloc(buf, (i + 2) * sizeof(char));
        buf[i++] = (char)c;
        buf[i] = '\0';
    }
    if (c == '\n' && !buf) { // if we saw a bare '\n' and never allocated, return ""
        buf = malloc(sizeof(char));
        buf[0] = '\0';
    }
    return buf;
}

int main(int argc, char * argv[]) {
    char * line;
    FILE * fp1 = fopen("test1", "r");
    FILE * fp2 = fopen("test2", "r");
    FILE * fp1out = fopen("result1", "w");
    FILE * fp2out = fopen("result2", "w");
    while (line = readline(fp1)) { //  NULL is “false” in C
        fputs(line, fp1out);
        putc('\n', fp1out);
        free(line);
    }
    while (line = readline(fp2)) { //  NULL is “false” in C
        fputs(line, fp2out);
        putc('\n', fp2out);
        free(line); 
    }
    fclose(fp1);
    fclose(fp2);
    fclose(fp1out);
    fclose(fp2out);
}