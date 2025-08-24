#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 1024
#define STACK_SIZE 100


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

int is_delim(char c, char * delimiters) {
    for (int i = 0; delimiters[i] != '\0'; i++) {
        if (c == delimiters[i]) {
            return 1;
        }
    }
    return 0;
}

int split_line(char * line, char ** items, char * delim) {
    int in_item = 0;
    int k = 0;
    for (int i = 0; line[i] != '\0'; i++) {
        if (!is_delim(line[i], delim) && !in_item) {
            items[k++] = &line[i];
            in_item = 1;
        }
        if (is_delim(line[i], delim)) {
            line[i] = '\0';
            in_item = 0;
        }
    }
    return k;
}

void push(int * stack, int * top_p, int value) {
    if (*top_p < STACK_SIZE - 1) {
        (*top_p)++;
        stack[*top_p] = value;
    }
}

int pop(int * stack, int * top_p) {
    if (*top_p >= 0) {
        (*top_p)--;
        return stack[*top_p + 1];
    } else {
        return 0;
    }
}

int parse_number(char ** pp) {
    int literal = 0;
    while ('0' <= **pp && **pp <= '9') {
        literal = literal * 10 + (**pp - '0');
        (*pp)++;
    }
    return literal;
}


void print_formatted(char ** items, int num_items, char * p) {
    int stack[STACK_SIZE];
    int top = -1;
    while (*p != '\0') {
        if (*p == '@') {
            p++;
            if (*p == '\0') {
                break;
            }
            if ('a' <= *p && *p <= 'z' && *p - 'a' < num_items) {
                printf("%s", items[*p - 'a']);
            }
            p++;
        } else if (*p == '{') { 
            p++;
            // evaluating expression
            while (*p != '}') {
                if ('a' <= *p && *p <= 'z') {
                    if ((*p - 'a' < num_items) && atoi(items[*p - 'a'])) { // from the docs:
                        push(stack, &top, atoi(items[*p - 'a']));
                    } else {
                        push(stack, &top, 0);
                    }
                } else if ('0' <= *p && *p <= '9') {
                    push(stack, &top, parse_number(&p));
                    continue;
                } else if ((*p == '+' || *p == '-' || *p == '*' || *p == '/') && (top >= 1)) {
                    int op2 = pop(stack, &top);
                    int op1 = pop(stack, &top);
                    switch (*p) {
                        case '+': push(stack, &top, op1 + op2); break;
                        case '-': push(stack, &top, op1 - op2); break;
                        case '*': push(stack, &top, op1 * op2); break;
                        case '/': push(stack, &top, op1 / op2); break;
                    }
                } 
                p++;
            }
            if (top >= 0) {
                printf("%d", pop(stack, &top));
            }
            p++;
        } else {
            putchar(*p);
            p++;
        }
    }
}


void process_file(FILE * fp, char * format, char * delim) {
    char line[MAXLEN];
    char * items[MAXLEN]; // array of char pointer, each pointing to the first char of an item in the line
    while (get_line(fp, line)) { // while file is not yet ended (i.e. there are still lines)
        int num_items = split_line(line, items, delim); // put NULLs between the items (according to delim chars) in line, save location of the first char of each item into items array
        print_formatted(items, num_items, format); // print items and expression results according to format string
        putchar('\n'); 
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