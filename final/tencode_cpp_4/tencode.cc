#include <iostream>
#include <sstream>
#include <cctype>
#include <string>  
#include <array> 
#include <vector> 
#include <map>
#include <algorithm> 
using namespace std;


/* I/O section */

string get_input() {
    char c;
    string s;
    while (cin.get(c)) {
        s.push_back(c);
    }
    return s;
}

string get_word(istream & in) { // std::istream is non-copyable => need to pass by reference!
    string s;
    char c;
    while ((in.get(c))) {
        if (!isalpha(c)) {
            in.unget();
            break;
        }
        s.push_back((char)c);
    }
    return s;
}


/* vocabulary section */

struct word {
    string str;
    int l;
    int c;
    int g;
};

vector<word> voc;
map<string, int> voc_idx;

void build_vocab(istream & in) {
    char c;
    string str;
    while (in.get(c)) {
        if (isalpha(c)) {
            in.unget();
            str = get_word(in); 
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

array<string, 128> code;

void compute_code() {
    for (int i = 0; i < 128; i++) {
        code[i] = (i < voc.size() && voc[i].g > 0) ? voc[i].str : "";
    }
}

int get_code(string str) {
    for (int i = 0; i < 128; i++) {
        if (!code[i].empty() && str == code[i]) {
            return i + 128;
        }
    }
    return -1;
}

void encode_text(istream & in) {
    char c;
    string str;

    // header
    for (int i = 0; i < 128 && !code[i].empty(); i++) {
        cout.put(i + 128);
        cout.write(code[i].c_str(), code[i].size());
    }
    if (!code[0].empty()) cout.put('\n');
    
    // body
    while (in.get(c)) {
        if (isalpha(c)) {
            in.unget();
            str = get_word(in);
            int b = get_code(str);
            if (b != -1) {
                cout.put(b);
            } else {
                cout.write(str.c_str(), str.size()); // formatted: out << str; raw: out.write(str.c_str(), str.size()); => use write for exact byte control
            }
        } else {
            cout.put(c);
        }
    }
}



/* main */

int main (int argc, char * argv[]) {
    string input = get_input();

    stringstream buffer(input);
    build_vocab(buffer); // pass 1

    compute_gains(); // compute gains
    sort(voc.begin(), voc.end(), [](const word& w1, const word& w2) {
        return (w1.g != w2.g) ? (w1.g > w2.g) : (w1.str < w2.str); // inline lambda, pass by reference (no copying)
    });
    compute_code(); // compute codes

    buffer.clear(); // remove eof/fail flags
    buffer.seekg(0, ios::beg); // rewind: move the get pointer (for reading) to position 0 relative to the beginning of the stream.
    encode_text(buffer); // pass 2
}