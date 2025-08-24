#include <cstdio>   // FILE, fopen, fclose, getc, putc, fputs, printf, stdin, stdout
#include <cstdlib>  // malloc, realloc, free
#include <cstring>  // strlen, strcmp
#include <cctype>   // isalpha
#include <vector>   // std::vector


/* I/O section */

char * get_input() {
    int c;
    char * s = (char *) malloc(sizeof(char));
    int i = 0;
    while ((c = getc(stdin)) != EOF) {
        s[i] = c;
        i++;
        s = (char *) realloc(s, (i + 1) * sizeof(char));
    }
    s[i] = '\0';
    return s;
}

char * get_word(FILE * fp) {
    int c;
    char * s = (char *) malloc(sizeof(char));
    int i = 0;
    while (isalpha((c = getc(fp)))) {
        s[i] = c;
        i++;
        s = (char *) realloc(s, (i + 1) * sizeof(char));
    }
    s[i] = '\0';
    if (c != EOF) ungetc(c, fp);
    return s;
}


/* vocabulary section */

struct word {
    char * str;
    int l;
    int c;
    int g;
};


std::vector <word> voc;

int lookup(char * str) {
    for (int i = 0; i < voc.size(); i++) {
        if (strcmp(str, voc[i].str) == 0) {
            return i; // found at location i
        }
    }
    return -1; // not found
}

void build_vocab(FILE * fp) {
    int c;
    char * str;
    while ((c = getc(fp)) != EOF) {
        if (isalpha(c)) {
            ungetc(c, fp);
            str = get_word(fp);
            int i = lookup(str);
            if (i > -1) { // if word was already seen
                free(str); // do not need this string anymore (word struct already contains one with the identical content)...
                voc[i].c++; // increase count
            } else { // otherwise
                struct word w = {str, strlen(str), 1};
                voc.push_back(w); // add word to the vocabulary
            }
        } 
    }
}

void init_gains() {
    for (int i = 0; i < voc.size(); i++) voc[i].g = voc[i].c * voc[i].l - voc[i].l - 1 - voc[i].c;
}

void sort_vocab() {
    for (int i = 0; i < voc.size(); i++) {
        for (int j = i + 1; j < voc.size(); j++) {
            if (voc[i].g < voc[j].g || (voc[i].g == voc[j].g && strcmp(voc[i].str, voc[j].str) > 0)) {
                struct word temp = voc[i];
                voc[i] = voc[j];
                voc[j] = temp;
            }
        }
    }
}

void print_vocab() {
    for (int i = 0; i < voc.size(); i++) {
        printf("count: %3d\tlength: %3d\tword: \"%s\"\n", voc[i].c, voc[i].l, voc[i].str);
    }
}

void delete_vocab() {
    for (int i = 0; i < voc.size(); i++) {
        free(voc[i].str);
        voc[i].str = NULL;
    }
    voc.clear(); // optional; vector will clean up on exit anyway
}


/* code secction */

char * code[128];

void init_code() {
    for (int i = 0; i < 128; i++) {
        code[i] = i < voc.size() && voc[i].g > 0 ? voc[i].str : NULL;
    }
}

int get_code(char * str) {
    for (int i = 0; i < 128; i++) {
        if (code[i] && strcmp(str, code[i]) == 0) {
            return i + 128;
        }
    }
    return -1;
}

void encode_text(FILE * fp) {
    int c;
    char * str;

    // header
    for (int i = 0; i < 128 && code[i]; i++) {
        putc(i + 128, stdout);
        fputs(code[i], stdout);
    }
    if (code[0]) putc('\n', stdout);
    
    // body
    while ((c = getc(fp)) != EOF) {
        if (isalpha(c)) {
            ungetc(c, fp);
            str = get_word(fp);
            int b = get_code(str);
            if (b != -1) {
                putc(b, stdout);
            } else {
                fputs(str, stdout);
            }
            free(str);
        } else {
            putc(c, stdout);
        }
    }
}

void delete_code() {
    for (int i = 0; i < 128; i++) {
        code[i] = NULL;
    }
}


/* main */

int main (int argc, char * argv[]) {
    char * input = get_input();

    FILE * input_fp = fopen("input", "w");
    fputs(input, input_fp);
    fclose(input_fp);
    free(input);

    FILE * fp = fopen("input", "r");
    build_vocab(fp);
    fclose(fp);

    init_gains();
    sort_vocab();
    init_code();

    fp = fopen("input", "r");
    encode_text(fp);
    fclose(fp);

    delete_code();
    delete_vocab();
}