#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAXLEN 1024



int is_delim(char c, char * delimeters) {
    for (int i = 0; delimeters[i] != '\0'; i++) {
        if (c == delimeters[i]) {
            return 1;
        }
    }
    return 0;
}


int get_line(FILE * fp, char * line) { // 1 if read a line, 0 on EOF
    int c;
    int i = 0;
    while ((c = getc(fp)) != EOF && i < MAXLEN - 1) { // need space for null; correction by chatGPT: read from fp using getc, not from stdin using getchar
        if (c == '\n') {
            break;
        }
        line[i++] = c;
    }
    line[i] = '\0';
    if (i == 0 && c == EOF) {
        return 0;
    }
    return 1;
} 


void process_file(FILE * fp, char * format, char * delim) {
    char line[MAXLEN];
    char * items[MAXLEN]; // array of char pointer
    while (get_line(fp, line)) { // while file is not yet ended / still has lines
        int processing_item = 0; // correction by chatGPT: reset processing_item for each line, not just once at the beginning of the file!!!!
        int k_item = 0;
        for (int i = 0; line[i] != '\0'; i++) {
            if (!is_delim(line[i], delim) && !processing_item) {
                items[k_item++] = &line[i];
                processing_item = 1;
            }
            if (is_delim(line[i], delim)) {
                line[i] = '\0';
                processing_item = 0;
            }
        }
        char * p = format;
        while (*p != '\0') {
            if (*p == '@') {
                p++;
                if (*p == '\0') {
                    break;
                }
                if ('a' <= *p && *p <= 'z' && *p - 'a' < k_item) {
                    printf("%s", items[*p - 'a']);
                }
                p++;
            } else {
                putchar(*p);
                p++;
            }
        }
        putchar('\n'); // correction by chatGPT: include this line
    }
}





int main(int argc, char * argv[]) {

    char default_format[] = "@a";
    char default_delim[] = " \t";

    char * format = default_format;
    char * delim = default_delim;

    int files_processed = 0;

    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "format=", 7) == 0) {
            format = argv[i] + 7;
        } else if (strncmp(argv[i], "delim=", 6) == 0) {
            delim = argv[i] + 6;
        } else {
            FILE * fp = NULL;
            if ((fp = fopen(argv[i], "r"))) {
                process_file(fp, format, delim);
                fclose(fp);
                files_processed++;
            }
        }
    }

    if (files_processed == 0) {
        process_file(stdin, format, delim);
    }


}





/* wisdom by chatGPT:
The main problems:
	1.	get_line(FILE *fp, ...) ignores fp and uses getchar() → you always read from stdin. Also c should be int (for EOF), and you should still return the last line if it ends without \n.
	2.	processing_item isn’t reset per line → the first field of the next line can get skipped.
	3.	You probably want a newline after each formatted line (examples imply line-wise output).
*/