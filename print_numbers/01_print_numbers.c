/*
// v1

#include <stdio.h>
#include <stdlib.h>

int print_numbers(const char * input) {
    const char * p = input;
    int numofnums = 0;
    while (*p != '\0') {
        if (*p >= '0' && *p <= '9' || *p == '+' && *(p+1) >= '0' && *(p+1) <= '9' || *p == '-' && *(p+1) >= '0' && *(p+1) <= '9') {
            putchar(*(p++));
            while (*p >= '0' && *p <= '9') {
                // NOT putchar(*p - '0'); // this would print the character with ascii code between 0 and 9, not the numbers
                putchar(*p);
                p++;
            }
            numofnums++;
            putchar('\n');
        } else {
            p++;
        }
    }
    return numofnums;
}

int main(int argc, char * argv[]) {
    int numofnums;
    for(int i = 0; i < argc; i++) {
        numofnums = print_numbers(argv[i]);
    }
}
*/



// v2 - suggestions from chatGPT

/*  
parsing with strtol: long val = strtol(p, &end, 10);
	•	Tries to parse a (possibly signed) base-10 integer starting at p.
	•	On entry, strtol skips any leading whitespace, then parses an optional +/- and as many consecutive digits as it can.
	•	It returns the parsed value in val.
	•	It writes into end the pointer to the first character not consumed as part of that number.
*/

#include <stdio.h>
#include <stdlib.h>

int print_numbers(const char * input) {
    const char * p = input;
    char * end; // will point to first character after parsed number
    int numofnums = 0;
    while (*p) {
        long num = strtol(p, &end, 10);
        if (*p == *end) {
            p++;
        } else {
            printf("%ld\n", num);
            numofnums++;
            p = end;
        }
    }
    return numofnums;
}

int main(int argc, char * argv[]) {
    int numofnums;
    for(int i = 1; i < argc; i++) { // argv[0] is the program name
        numofnums = print_numbers(argv[i]);
    }
}