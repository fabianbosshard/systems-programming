#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024
#define MAX_NUM_ITEMS 26

int is_delim(char c, const char * delim) {
    for (int i = 0; delim[i] != '\0'; i++) {
        if (c == delim[i]) {
            return 1;
        }
    }
    return 0;
}

int tokenize_line(char * tokens[], char * line, const char * delim) {
    int item_state = 0;
    int num_item = 0;
    for (int i = 0; line[i] != '\0'; i++) {
        if (is_delim(line[i], delim)) {
            line[i] = '\0';
            item_state = 0;
            if (num_item == MAX_NUM_ITEMS) {
                return MAX_NUM_ITEMS;
            }
        } else {
            if (item_state == 0) {
                tokens[num_item++] = &line[i];
            }
            item_state = 1;
        }
    }
    return num_item;
}

void print_tokens(char * tokens[], int num_tokens, const char * format) {
    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] == '@') {
            int idx = format[++i] - 'a';
            if (0 <= idx && idx < num_tokens) {
                fputs(tokens[idx], stdout);
            }
        } else {
            putchar(format[i]);
        }
    }
    putchar('\n');
}


void process_file(FILE * fp, const char * format, const char * delim) {
    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, fp)) {

        int line_length = strlen(line);
        if (line_length > 0 && line[line_length - 1] == '\n') {
            line[line_length-- - 1] = '\0';
        }

        char * tokens[MAX_NUM_ITEMS];
        int num_tokens = 0;
        num_tokens = tokenize_line(tokens, line, delim);

        print_tokens(tokens, num_tokens, format);
    }
}


int main(int argc, char * argv[]) {
    const char * format = "@a";
    const char * delim = " \t";
    unsigned nr_files = 0;

    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "format=", strlen("format=")) == 0) {
            format = argv[i] + strlen("format=");
        } else if (strncmp(argv[i], "delim=", strlen("delim=")) == 0) {
            delim = argv[i] + strlen("delim=");
        } else {
            FILE * fp = fopen(argv[i], "r");
            if (!fp) {
                continue;
            } else {
                process_file(fp, format, delim);
                fclose(fp);
            }
        }
    }

    if (nr_files == 0) {
        process_file(stdin, format, delim);
    }


}