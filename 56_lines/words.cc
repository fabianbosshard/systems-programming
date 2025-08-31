#include <iostream>
#include <string>
#include <map>
#include <set>
#include <cctype>

#include "words.h"

struct lines {
    std::map<unsigned, std::string> M;
    unsigned next_id;
};

struct lines * lines_create() {
    struct lines * l = new struct lines;
    l->next_id = 0;
    return l;
}

void lines_destroy (struct lines * l) {
    delete l;
}

int add_line (struct lines * l, const char * line) {
    unsigned id = l->next_id++;
    l->M[id] = std::string(line);
    return id;
}

int remove_line (struct lines * l, int id) {
    if (l->M.find(id) == l->M.end()) {
        return false;
    }
    l->M.erase(id);
    return true;
}

const char * get_word_start(const char * p) {
    while (isspace(*p) && *p != '\0') {
        p++;
    }
    return (*p) ? p : NULL; // if *p is the string terminator '\0', there are no more words in p...
}
const char * get_word_end(const char * q) {
    while (!isspace(*q) && *q != '\0') {
        q++;
    }
    return q;
}
std::set<std::string> parse_words(const char * words) {
    const char * p = words;
    const char * q = words;
    std::set<std::string> W;
    while ((p = get_word_start(p)) != NULL) {
        q = get_word_end(p);
        std::string w(p, q);
        W.insert(w);
        p = q;
    }
    return W;
}
bool match(std::string X, std::set<std::string> W) {
    if (W.empty()) return false; // empty set of words never matches a string...
    for (auto w : W) {
        if (X.find(w) == std::string::npos) { // X does not contain w as a substring
            return false;
        }
    }
    return true;
}
int match (struct lines * l, const char * output[], int max_output, const char * words) {
    unsigned i = 0;
    std::set<std::string> W = parse_words(words);
    for (auto & p : l->M) { // IMPORTANT: iterate by reference (i.e. using the reference declarator `&`)!!! otherwise `auto p` copies each map element and `p.second.c_str()` points into that temporary copy, which is destroyed at the end of that loop iteration. That leaves `output[i]` dangling → garbage
        if (match(p.second, W)) {
            output[i] = p.second.c_str();
            i++;
            if (i == max_output) {
                break;
            }
        }
    }
    return i;
}
