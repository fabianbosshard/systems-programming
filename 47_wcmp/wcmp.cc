#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>

#include <iostream>
#include <sstream>
#include <string>
#include <array> 
#include <vector> 
#include <map>
#include <algorithm>
using namespace std; 


string get_word(const char * * p) {
    string w;
    while (!isalpha(**p)) {
        if (**p != '\0') {
            (*p)++;
        } else {
            return w;
        }
    }
    while (isalpha(**p)) {
        w += **p;
        (*p)++;
    }
    return w;
}

vector<string> get_sequence(const char * p) {
    string str;
    vector<string> seq;
    while ((str = get_word(&p)).length() > 0) {
        seq.push_back(str);
    }
    return seq;
}

int wcmp(const char * a, const char * b) {
    vector<string> words_a = get_sequence(a);
    vector<string> words_b = get_sequence(b);
    for (int i = 0; i < words_a.size() && i < words_b.size(); i++) {
        if (words_a[i] > words_b[i]) {
            return 1;
        }
        if (words_a[i] < words_b[i]) {
            return -1;
        }
    }
    if (words_a.size() > words_b.size()) {
        return 1;
    }
    if (words_a.size() < words_b.size()) {
        return -1;
    }
    return 0;
}