#include "../57_colors/colors.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>


class Style {
    public:
    std::string ft;
    std::string fg;
    std::string bg;
    Style() {
        ft = "fixed";
        fg = "#000000";
        bg = "#FFFFFF";
    }
};


void process_input(std::istream & input_stream, std::map<std::string, Style> & styles, std::map<std::string, std::string> & colors) {
    std::string line;
    while (std::getline(input_stream, line)) {
        auto line_input = std::stringstream(line);
        std::string word;
        if (!(line_input >> word)) continue; // skip empty lines
        if (word == "color") {
            std::string colorname, colorcode;
            if (!(line_input >> colorname)) exit(EXIT_FAILURE);
            if (line_input >> colorcode) {
                colors[colorname] = colorcode;
            } else exit(EXIT_FAILURE);
            if (line_input >> word) exit(EXIT_FAILURE); // reject trailing junk on color lines
        } else if (word == "style") {
            std::string stylename;
            if (!(line_input >> stylename)) exit(EXIT_FAILURE);
            Style & style = styles[stylename]; // start from existing entry (or create it if it does not exist yet): then overwrite fg/bg/ft if the attribute appears
            while (line_input >> word) {
                if (word == "foreground") {
                    if (line_input >> word) {
                        style.fg = word;
                    } else exit(EXIT_FAILURE);
                } else if (word == "background") {
                    if (line_input >> word) {
                        style.bg = word;
                    } else exit(EXIT_FAILURE);
                } else if (word == "font") {
                    if (line_input >> word) {
                        style.ft = word;
                    } else exit(EXIT_FAILURE);
                } else exit(EXIT_FAILURE);
            }
        } else exit(EXIT_FAILURE);
    }
}

void fix_color(std::string & col_attr, std::map<std::string, std::string> & colors) {
    if (col_attr[0] != '#') {
        std::string col_value;
        if (colors.find(col_attr) != colors.end()) {
            col_value = colors[col_attr];
        } else exit(EXIT_FAILURE);
        col_attr = "#" + col_value;
    }
}
void resolve_colors(std::map<std::string, Style> & styles, std::map<std::string, std::string> & colors) {
    for (auto & s : styles) {
        fix_color(s.second.fg, colors);
        fix_color(s.second.bg, colors);
    }
}

void output_styles(std::map<std::string, Style> & styles) {
    for (auto & s : styles) {
        std::cout << s.first << ": ";
        struct color co; 

        std::cout << "fg=";
        string_to_color(&co, &((s.second.fg.c_str())[1]));
        std::cout << co.red << "," << co.green << "," << co.blue << " ";

        std::cout << "bg=";
        string_to_color(&co, &((s.second.bg.c_str())[1]));
        std::cout << co.red << "," << co.green << "," << co.blue << " ";

        std::cout << "ft=";
        std::cout << s.second.ft << std::endl;
    }
}


int main(int argc, char * argv[]) {

    std::map<std::string, Style> styles;
    std::map<std::string, std::string> colors;

    if (argc == 1) {
        process_input(std::cin, styles, colors);
    }
    for (int i = 1; i < argc; i++) {
        std::ifstream input_stream(argv[i]);
        input_stream ? process_input(input_stream, styles, colors) : exit(EXIT_FAILURE);
    }

    resolve_colors(styles, colors);

    output_styles(styles);
}
