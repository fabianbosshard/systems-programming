#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <string>    // std::string
#include <vector>    // std::vector
#include <map>       // std::map
#include <algorithm> // std::sort


/* I/O section */

std::string get_input() {
    int c;
    std::string s;
    while ((c = getc(stdin)) != EOF) {
        s.push_back((char)c);
    }
    return s;
}

std::string get_word(FILE * fp) {
    std::string s;
    int c;
    while (isalpha((c = getc(fp)))) {
        s.push_back((char)c);
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

std::vector <word> voc;
std::map <std::string, int> voc_idx;

void build_vocab(FILE * fp) {
    int c;
    std::string str;
    while ((c = getc(fp)) != EOF) {
        if (isalpha(c)) {
            ungetc(c, fp);
            str = get_word(fp);
            if (voc_idx.find(str) == voc_idx.end()) { // if word has not been encountered before
                struct word w = {str, (int)str.size(), 1};
                voc.push_back(w); // add word to the vocabulary
                voc_idx[w.str] = voc.size() - 1;
            } else {  // if word was already seen
                voc[voc_idx[str]].c++; // increase count
            }
        } 
    }
}

void compute_gains() {
    for (int i = 0; i < voc.size(); i++) voc[i].g = voc[i].c * voc[i].l - voc[i].l - 1 - voc[i].c;
}


/* code secction */

std::string code[128];

void compute_code() {
    for (int i = 0; i < 128; i++) {
        code[i] = (i < voc.size() && voc[i].g > 0) ? voc[i].str : "";
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

    FILE * fp = tmpfile(); // unnamed temporary file (instead of creating one on disk)
    fputs(input.c_str(), fp);

    rewind(fp);
    build_vocab(fp); // pass 1

    compute_gains(); // compute gains
    std::sort(voc.begin(), voc.end(), [](const word& w1, const word& w2) {
        return (w1.g != w2.g) ? (w1.g > w2.g) : (w1.str < w2.str); // inline lambda, pass by reference (no copying)
    });
    compute_code(); // compute codes

    rewind(fp);
    encode_text(fp); // pass 2
    fclose(fp);
}