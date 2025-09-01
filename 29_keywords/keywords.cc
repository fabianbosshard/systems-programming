
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <tuple>
#include <map>
#include <algorithm>
#include <cctype>


class Keywords {

private:
    struct word {
        std::string name;
        unsigned count;
        unsigned freq;
    };
    std::istream & input;
    std::ostream & output;

    bool get_word(std::string & w) {
        w.clear();
        char c;
        while (input.get(c)) {
            if (isalpha(c)) {
                input.unget();
                break;
            }
        }
        while (input.get(c)) {
            if (isalpha(c)) {
                w.push_back(c);
            } else {
                return true;
            }
        }
        return false;
    }

public:
    unsigned num_words;
    unsigned low;
    unsigned high;
    bool fancy;
    bool reverse;

    Keywords(std::istream & input_, std::ostream & output_) : input(input_), output(output_) { // initializer list: some members cannot be assigned to after the object is created (references, const members, members of types with no assignment operator). initializer lists solve that by constructing or binding those members right when the object is being built.
        low = 10;
        high = 90;
        fancy = false;
        reverse = false;
        num_words = 0;
    } 

    std::map<std::string, unsigned> count;
    void process_input() {
        std::string w;
        while (get_word(w)) {
            num_words++;
            if (count.find(w) == count.end()) {
                count[w] = 1;
            } else {
                count[w]++;
            }
        }
    }

    std::vector<struct word> keywords;
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
        std::sort(keywords.begin(), keywords.end(), [this](struct word a, struct word b) {
            if (a.count == b.count) { // in std::sort, the comparator must answer: “should a come before b?” for ascending order, the smaller value should return true
                return a.name < b.name;
            }
            if (reverse) {
                return a.count < b.count;
            } else {
                return a.count > b.count;
            }
        });
    }

    void output_frequencies() {
        for (auto & w : keywords) {
            if (w.freq >= low && w.freq <= high) {
                output << " ";
                if (fancy) {
                    output << w.count << ":";
                }
                output << w.name;
            }
        }
        output << std::endl;
    }
};

int main(int argc, char * argv[]) {

    bool processing_parameters = true;
    unsigned files_processed = 0;

    unsigned low = 10;
    unsigned high = 90;
    bool fancy = false;
    bool reverse = false;

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
            Keywords keywords(input_stream, std::cout);
            keywords.low = low;
            keywords.high = high;
            keywords.fancy = fancy;
            keywords.reverse = reverse;
            keywords.process_input();
            keywords.calculate_frequencies();
            keywords.sort_keywords();
            keywords.output_frequencies();
            files_processed++;
        }
    }
    if (files_processed == 0) {
        Keywords keywords(std::cin, std::cout);
        keywords.low = low;
        keywords.high = high;
        keywords.fancy = fancy;
        keywords.reverse = reverse;
        keywords.process_input();
        keywords.calculate_frequencies();
        keywords.sort_keywords();
        keywords.output_frequencies();
    }
}
