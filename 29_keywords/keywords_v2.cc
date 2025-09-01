
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>


struct word {
    std::string name;
    unsigned count;
    unsigned freq;
};

bool get_word(std::istream & input, std::string & w) {
    w.clear();
    char c;
    while (input.get(c)) {
        if (!isalpha(c)) {
            if (!w.empty()) return true;
        } else {
            w.push_back(c);
        }
    }
    return !w.empty() ? true : false;
}

unsigned num_words = 0;
unsigned low = 10;
unsigned high = 90;
bool fancy = false;
bool reverse = false;

std::map<std::string, unsigned> count;
std::vector<struct word> keywords;

void process_input(std::istream & input) {
    std::string w;
    while (get_word(input, w)) {
        num_words++;
        count.find(w) == count.end() ? count[w] = 1 : count[w]++;
    }
}

void calculate_frequencies() {
    for (auto & p : count) {
        struct word keyword;
        keyword.name = p.first;
        keyword.count = p.second;
        keyword.freq = (100 * keyword.count) / num_words;
        keywords.push_back(keyword);
    }
}

void sort_keywords() {
    std::sort(keywords.begin(), keywords.end(), 
        [](struct word a, struct word b) {
            return (a.count == b.count) ? (a.name < b.name) : (reverse ? a.count < b.count : a.count > b.count); // in std::sort, the comparator must answer: “should a come before b?” for ascending order, the smaller value should return true
        }
    );
}

void output_frequencies(std::ostream & output) {
    for (auto & w : keywords) {
        if (w.freq >= low && w.freq <= high) {
            output << " ";
            if (fancy) output << w.count << ":";
            output << w.name;
        }
    }
    output << std::endl;
}

void run(std::istream & input, std::ostream & output) {
    count.clear();
    keywords.clear();
    num_words = 0;
    process_input(input);
    calculate_frequencies();
    sort_keywords();
    output_frequencies(output);
}

int main(int argc, char * argv[]) {

    bool processing_parameters = true;
    unsigned files_processed = 0;

    for (int i = 1; i < argc; i++) {
        if (processing_parameters) {
            if (std::string(argv[i]).compare(0, 4, "low=") == 0) {
                low = std::stoi(std::string(argv[i] + 4));
            } else if (std::string(argv[i]).compare(0, 5, "high=") == 0) {
                high = std::stoi(std::string(argv[i] + 5));
            } else if (std::string(argv[i]).compare(0, 2, "-f") == 0) {
                fancy = true;
            } else if (std::string(argv[i]).compare(0, 2, "-r") == 0) {
                reverse = true;
            } else if (std::string(argv[i]).compare(0, 2, "--") == 0) {
                processing_parameters = false;
            } else {
                i--;
                processing_parameters = false;
            }
        } else {
            std::ifstream input_stream(argv[i]);
            std::cout << argv[i];
            run(input_stream, std::cout);
            files_processed++;
        }
    }
    if (files_processed == 0) {
        run(std::cin, std::cout);
    }
}
