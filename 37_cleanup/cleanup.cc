#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <cctype>

#include <dirent.h>





std::map<std::string, std::set<std::string>> read_rules() {
    /*
    create empty map from string to set of string (source file -> derived files)

    while (there are lines) {
        (maybe make stream from line)

        read characters from the line and put the into a string "source" until a dot is encountered

        create empty set "derived"
        while we are not at the end of the line {
            split string by dots and put each one into the set "derived"
        }

        add "source" -> "derived" to the map
    
    }

    return map
    */
    std::ifstream input_stream("RULES");
    std::string line;
    std::map<std::string, std::set<std::string>> R;
    while (std::getline(input_stream, line)) {
        auto line_input = std::stringstream(line + " "); // simplify logic
        char c = 0;
        std::string source_extension;
        while (line_input.get(c)) {
            if (c == '.') {
                c = 0;
                break;
            }
            source_extension.push_back(c);
        }
        std::set<std::string> derived_extensions;
        std::string temp;
        while (line_input.get(c)) {
            if (c == '.' || isblank(c) || isspace(c)) {
                c = 0;
                derived_extensions.insert(temp);
                temp.clear();
            } else {
                temp.push_back(c);
            }
        }
        R[source_extension] = derived_extensions;
    }
    return R;
}


void explore_directory(char * dirname, std::map<std::string, std::set<std::string>> & R) {
    /*
    for item in directory 
        if item is regular file and has extension (has one dot that is not at the last position)
            get extension adn put into string
            find extension in map.first
            if found
                put extension into 
                get name
                put name into string
                for item in directory
                    if get_name(item) == name and get_extension(item) == extension
                        print item including directory path

    */

    // build set of strings from the directory
    std::set<std::string> filenames;
    DIR * dirp = opendir(dirname);
    if (dirp == NULL) exit(EXIT_FAILURE);
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {
        std::string filename(dp->d_name);
        if (filename.find(".") != std::string::npos && filename.find(".") == filename.rfind(".") && filename.rfind(".") > 0 && filename.rfind(".") < filename.size() - 1) {
            filenames.insert(filename);
        }
    }
    closedir(dirp);


    for (auto & source_file : filenames) {
        std::string source_name = source_file.substr(0, source_file.find("."));
        std::string source_extension = source_file.substr(source_file.find(".") + 1);
        if (R.find(source_extension) != R.end()) {
            for (auto & file : filenames) {
                std::string derived_name = file.substr(0, file.find("."));
                std::string derived_extension = file.substr(file.find(".") + 1);
                if (derived_name == source_name && R[source_extension].find(derived_extension) != R[source_extension].end()) {
                    std::cout << dirname << "/" << file << std::endl;
                }
            }
        }
    }

}


int main(int argc, char * argv[]) {

    std::map<std::string, std::set<std::string>> rules = read_rules();

    // for (auto & p : rules) {
    //     std::cout << p.first << ":" << std::endl;
    //     for (auto & d : p.second) {
    //         std::cout << d << "|";
    //     }
    //     std::cout << std::endl;
    // }


    for (int i = 1; i < argc; i++) {
        explore_directory(argv[i], rules);
    }

    

}
