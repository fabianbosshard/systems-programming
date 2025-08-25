#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <vector>
#include <string>


/* I/O section */

std::string get_input() {
    int c;
    std::string s = "";
    while ((c = getc(stdin)) != EOF) {
        s.push_back(c);
    }
    return s;
}

std::string get_word(FILE * fp) {
    int c;
    std::string s = "";
    while (isalpha((c = getc(fp)))) {
        s.push_back(c);
    }
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

std::vector <struct word> voc;

int lookup(std::string str) {
    for (int i = 0; i < voc.size(); i++) {
        if (str == voc[i].str) {
            return i; // found at location i
        }
    }
    return -1; // not found
}


void build_vocab(FILE * fp) {
    int c;
    std::string str;
    while ((c = getc(fp)) != EOF) {
        if (isalpha(c)) {
            ungetc(c, fp);
            str = get_word(fp);
            int i = lookup(str);
            if (i > -1) { // if word was already seen
                voc[i].c++; // increase count
            } else { // otherwise
                struct word w = {str, str.length(), 1};
                voc.push_back(w); // add word to vocabulary
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
            if (voc[i].g < voc[j].g || (voc[i].g == voc[j].g && voc[i].str > voc[j].str)) {
                std::swap(voc[i], voc[j]);
            }
        }
    }
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
        code[i] = i < voc.size() && voc[i].g > 0 ? voc[i].str : "";
    }
}

int get_code(std::string str) {
    for (int i = 0; i < 128; i++) {
        if (!code[i].empty() && str == code[i]) {
            return i + 128;
        }
    }
    return -1;
}

void encode_text(FILE * fp) {
    int c;
    std::string str;

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
                fputs(str.c_str(), stdout);
            }
        } else {
            putc(c, stdout);
        }
    }
}



/* main */

int main (int argc, char * argv[]) {
    std::string input = get_input();

    FILE * fp = tmpfile();
    fputs(input.c_str(), fp);

    rewind(fp);

    build_vocab(fp);

    rewind(fp);

    init_gains();
    sort_vocab();
    init_code();
    encode_text(fp);
    
    fclose(fp);
}