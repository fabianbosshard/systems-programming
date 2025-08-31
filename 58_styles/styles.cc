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
    Style() {
        nam = "";
        ft = "fixed";
        fg = "#000000";
        bg = "#FFFFFF";
    }
};


void process_input(std::istream & in, std::map<std::string, Style> & styles, std::map<std::string, std::string> & colors) {
    std::string line;
    while(std::getline(in, line)) {
        auto in = std::stringstream(line);
        std::string word;
        if (!(in >> word)) continue; // skip empty lines
        if (word == "color") {
            std::string colorname, colorcode;
            if (!(in >> colorname)) exit(EXIT_FAILURE);
            if (in >> colorcode) {
                colors[colorname] = colorcode;
            } else exit(EXIT_FAILURE);
        } else if (word == "style") {
            std::string stylename;
            if (!(in >> stylename)) exit(EXIT_FAILURE);
            Style style;
            style.nam = stylename;
            while (in >> word) {
                if (word == "foreground") {
                    if (in >> word) {
                        style.fg = word;
                    } else exit(EXIT_FAILURE);
                } else if (word == "background") {
                    if (in >> word) {
                        style.bg = word;
                    } else exit(EXIT_FAILURE);
                } else if (word == "font") {
                    if (in >> word) {
                        style.ft = word;
                    } else exit(EXIT_FAILURE);
                } else {
                    exit(EXIT_FAILURE);
                }
            }
            styles[stylename] = style;
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

    std::map<std::string, Style> styles;
    std::map<std::string, std::string> colors;

    if (argc == 1) {
        process_input(std::cin, styles, colors);
    }
    for (int i = 1; i < argc; i++) {
        std::ifstream in(argv[i]);
        in ? process_input(in, styles, colors) : exit(EXIT_FAILURE);
    }

    std::vector<Style> styles_v;
    for (auto & s_pair : styles) {
        styles_v.push_back(s_pair.second);
    }

    std::sort(styles_v.begin(), styles_v.end(), compare);


    output_styles(styles_v, colors);

}
