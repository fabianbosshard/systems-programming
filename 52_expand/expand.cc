#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

unsigned colons(std::string & s) {
    unsigned N = 0;
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == ':') N++;
    }
    return N;
}

void get_rules(char * filename, std::vector<std::string> & P, std::vector<std::string> & T) {
    std::ifstream input(filename);
    if (!input) {
        std::cout << "invalid rules file " << filename << std::endl;
        exit(EXIT_FAILURE);
    }
    std::string line;
    bool in_rule = false;
    std::string p, t;
    while (std::getline(input, line)) {
        if (colons(line) > 0) {
            if (in_rule) {
                if (line[0] == ':') {
                    line[0] = '\n';
                    t += line;
                } else {
                    P.push_back(p);
                    T.push_back(t);
                    p = line.substr(0, line.find(":"));
                    t = line.substr(line.find(":") + 1);
                }
            } else {
                if (line[0] == ':') {
                    std::cout << "invalid rules file " << filename << std::endl;
                    exit(EXIT_FAILURE);
                } else {
                    in_rule = true;
                    p = line.substr(0, line.find(":"));
                    t = line.substr(line.find(":") + 1);
                }
            }
        }
    }
    if (in_rule) {
        P.push_back(p);
        T.push_back(t);
    }
}



void get_match(std::string & line, unsigned & i, std::vector<std::string> & P, std::vector<std::string> & T, std::ostream & output) {
    for (int j = 0; j < P.size(); j++) {
        if (line.substr(i, P[j].length()) == P[j]) {
            output << T[j];
            i += P[j].length() - 1;
            return;
        }
    }
    output << line[i];
}

void expand(std::istream & input, std::ostream & output, std::vector<std::string> & P, std::vector<std::string> & T) {
    std::string line;
    while (std::getline(input, line)) {
        for (unsigned i = 0; i < line.length(); i++) {
            get_match(line, i, P, T, output);
        }
        output << std::endl;
    }
}


int main(int argc, char * argv[]) {

    std::vector<std::string> P;
    std::vector<std::string> T;

    if (argc == 1) {
        get_rules("RULES", P, T);
    }
    for (int i = 1; i < argc; i++) {
        std::ifstream input_stream(argv[i]);
        get_rules(argv[i], P, T);
    }

    expand(std::cin, std::cout, P, T);
}
