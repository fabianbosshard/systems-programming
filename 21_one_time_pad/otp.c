#include <stdio.h>
#include <stdlib.h>
#include <string.h>


long unsigned encrypt_file(FILE * inp, FILE * outp, FILE * keyf, long unsigned count) {
    int x_pre;
    char x, k, y;
    fseek(keyf, count, SEEK_SET);
    while ((x_pre = getc(inp)) != EOF) {
        k = getc(keyf);
        x = (char)x_pre;
        y = x ^ k;
        putc(y, outp);
        count++;
    }
    return count;
}

int main(int argc, char * argv[]) {

    char default_keyfile[] = "key";
    char counter_string[] = ".counter";
    char * keyfile_name = default_keyfile;
    long unsigned counter = 0; // initialize to avoid garbage

    FILE * input = stdin;
    FILE * output = stdout;

    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "in=", 3) == 0) {
            input = fopen(argv[i] + 3, "r");
        } else if (strncmp(argv[i], "out=", 4) == 0) {
            output = fopen(argv[i] + 4, "w");
        } else if (strncmp(argv[i], "key=", 4) == 0) {
            keyfile_name = argv[i] + 4;
        } else {
            exit(EXIT_FAILURE);
        }
    }

    FILE * keyfile = fopen(keyfile_name, "r");

    unsigned counterfile_name_len = strlen(keyfile_name) + strlen(counter_string);
    char * counterfile_name = malloc((counterfile_name_len + 1) * sizeof(char));
    for (int i = 0; i < counterfile_name_len; i++) {
        if (i < strlen(keyfile_name)) {
            counterfile_name[i] = keyfile_name[i];
        } else {
            counterfile_name[i] = counter_string[i - strlen(keyfile_name)];
        }
    }
    counterfile_name[counterfile_name_len] = '\0';

    // "rw" does not exitst!!! The argument mode points to a string beginning with one of the following letters: “r”     Open for reading.  The stream is positioned at the beginning of the file.  Fail if the file does not exist. “w”     Open for writing.  The stream is positioned at the beginning of the file.  Truncate the file to zero length if it exists or create the file if it does not exist. “a”     Open for writing.  The stream is positioned at the end of the file.  Subsequent writes to the file will always end up at the then current end of file, irrespective of any intervening fseek(3) or similar. Create the file if it does not exist. An optional “+” following “r”, “w”, or “a” opens the file for both reading and writing. 
    FILE * counterfile = fopen(counterfile_name, "r+");
    if (!counterfile) counterfile = fopen(counterfile_name, "w+");
    if (!fscanf(counterfile, "%lu", &counter)) { // use address of counter !!!!!!!!!!!
        counter = 0;
    }

    counter = encrypt_file(input, output, keyfile, counter);

    rewind(counterfile);
    fprintf(counterfile, "%lu\n", counter); // "lu", not "ud"
    fflush(counterfile);

    free(counterfile_name);
}
