#include "../57_colors/colors.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <set>
#include <vector>
#include <map>




class Style {
    public:
    std::string nam;
    std::string ft;
    std::string fg;
    std::string bg;
    Style(std::string name) {
        nam = name;
        ft = "fixed";
        fg = "#000000";
        bg = "#FFFFFF";
    }
};



void process_input(std::istream & in, std::vector<Style> & styles, std::map<std::string, std::string> & colors) {
    std::string line;
    while(std::getline(in, line)) {
        if (line.length() == 0) continue;
        auto in = std::stringstream(line);
        std::string word;
        in >> word;
        if (word == "color") {
            std::string colorname, colorcode;
            in >> colorname >> colorcode;
            colors[colorname] = colorcode;
        } else if (word == "style") {
            std::string stylename;
            in >> stylename;
            Style style(stylename);
            while (in >> word) {
                if (word == "foreground") {
                    in >> word;
                    style.fg = word;
                } else if (word == "background") {
                    in >> word;
                    style.bg = word;
                } else if (word == "font") {
                    in >> word;
                    style.ft = word;
                } else {
                    exit(EXIT_FAILURE);
                }
            }
            styles.push_back(style);
        } else {
            exit(EXIT_FAILURE);
        }

    }
}


bool compare(Style a, Style b) {
    return a.nam < b.nam;
}


void output_styles(std::vector<Style> & styles, std::map<std::string, std::string> & colors) {
    for (auto & s : styles) {
        std::cout << s.nam << ": ";

        std::cout << "fg=";
        struct color * fg_p = (struct color *) malloc(sizeof(struct color));
        if (s.fg[0] == '#') {
            string_to_color(fg_p, &((s.fg.c_str())[1]));
        } else {
            std::string foreground;
            if (colors.find(s.fg) != colors.end()) {
                foreground = colors[s.fg];
            } else exit(EXIT_FAILURE);
            string_to_color(fg_p, &((foreground.c_str())[0]));
        }
        std::cout << fg_p->red << "," << fg_p->green << "," << fg_p->blue << " ";
        free(fg_p);

        std::cout << "bg=";
        struct color * bg_p = (struct color *) malloc(sizeof(struct color));
        if (s.bg[0] == '#') {
            string_to_color(bg_p, &((s.bg.c_str())[1]));
        } else {
            std::string background;
            if (colors.find(s.bg) != colors.end()) {
                background = colors[s.bg];
            } else exit(EXIT_FAILURE);
            string_to_color(bg_p, &((background.c_str())[0]));
        }
        std::cout << bg_p->red << "," << bg_p->green << "," << bg_p->blue << " ";
        free(bg_p);

        std::cout << "ft=";
        std::cout << s.ft << std::endl;
    }
}


int main(int argc, char * argv[]) {

    std::vector<Style> styles;
    std::map<std::string, std::string> colors;

    if (argc == 1) {
        process_input(std::cin, styles, colors);
    }
    for (int i = 1; i < argc; i++) {
        std::ifstream in(argv[i]);
        in ? process_input(in, styles, colors) : exit(EXIT_FAILURE);
    }

    std::sort(styles.begin(), styles.end(), compare);


    output_styles(styles, colors);

}
