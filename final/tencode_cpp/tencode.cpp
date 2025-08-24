#include <cstdio>    // FILE, fopen, fclose, getc, putc, fputs, printf, stdin, stdout
#include <cstdlib>   // malloc, realloc, free
#include <cstring>   // strlen
#include <cctype>    // isalpha
#include <string>    // std::string
#include <vector>    // std::vector
#include <algorithm> // std::sort


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
    std::string str;
    int l;
    int c;
    int g;
};


std::vector <word> voc;

int lookup(char * str) {
    for (int i = 0; i < voc.size(); i++) {
        if (str ==  voc[i].str) {
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

bool word_compare(struct word w1, struct word w2) {
    return w1.g > w2.g || (w1.g == w2.g && w1.str < w2.str);
}

void print_vocab() {
    for (int i = 0; i < voc.size(); i++) {
        printf("count: %3d\tlength: %3d\tword: \"%s\"\n", voc[i].c, voc[i].l, voc[i].str.c_str());
    }
}


/* code secction */

std::string code[128];

void init_code() {
    for (int i = 0; i < 128; i++) {
        code[i] = (i < voc.size() && voc[i].g > 0) ? voc[i].str : "";
    }
}

int get_code(char * str) {
    for (int i = 0; i < 128; i++) {
        if (!code[i].empty() && str == code[i]) {
            return i + 128;
        }
    }
    return -1;
}

void encode_text(FILE * fp) {
    int c;
    char * str;

    // header
    for (int i = 0; i < 128 && !code[i].empty(); i++) {
        putc(i + 128, stdout);
        fputs(code[i].c_str(), stdout);
    }
    if (!code[0].empty()) putc('\n', stdout);
    
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
    std::sort(voc.begin(), voc.end(), word_compare);
    init_code();

    fp = fopen("input", "r");
    encode_text(fp);
    fclose(fp);
}