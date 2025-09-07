# Systems Programming


## Exam Exercises

### [string stack](03_strstack/strstack.c) (Exercise 3)

### [Path Manipulation](04_path/path.c) (Exercise 4)

### [Set of Integers](13_set/cset.c) (Exercise 13)
implements a set of integers using a direct-addressing table

### [One-Time Pad Encryption](21_one_time_pad/otp.c) (Exercise 21)

### [Average of positive Integers](22_average/avg.c) (Exercise 22)

### [Keywords](29_keywords/keywords_v2.cc) (Exercise 29)
- initializer list (OOP in C++): some members cannot be assigned to after the object is created (references, const members, members of types with no assignment operator). initializer lists solve that by constructing or binding those members right when the object is being built.

### [Geometric Map](30_geomap/geomap.cc) (Exercise 30)
uses `std::list` of structs

### [Table of String Records](31_string_records) (Exercise 31)
 manages a table of string records
- [s_table](31_string_records/s_table_cpp1/): `new`, `delete`, `auto`, exceptions (`try {} catch (...) {}`), `std::list`, iterators (`begin()`, `end()`)
- [s_table](31_string_records/s_table_cpp1/): uses a bit more C++

### [Messaging System](34_35_messaging) (Exercise 34/35)
simple publish/subscribe messaging system where a server records the itnerests of receivers and then delivers messages according to those interests
- [single tag (C)](34_35_messaging/34_messaging_single_tag/messaging.c): doubly linked list in C
- [multiple tags (C++)](34_35_messaging/35_messaging_multi_tag/messaging.cc): `std::set`, `std::map`, `std::string`, `new`, `delete`
- [multiple tags (C++)](34_35_messaging/35_messaging_multi_tag_withFSM/messaging.cc) with diffent function to parse tags (FSM based)

### [Simple Sets](36_simplesets/simplesets.c) (Exercise 36)
sets have an identity, if they are merged, they are the same set afterwards

### [Cleanup: detect derived files](37_cleanup/cleanup.cc) (Exercise 37)
- `dirent.h` library for managing directories: `opendir()`, `readdir()`, `closedir()`, `DIR *`, `struct dirent *`
- string manipulation with `.find()`, `.substr()`, `std::string::npos`
- `std::map<std::string, std::set<std::string>>` to map from source file extension to set of derived file extensions

### [Tank Control System](38_tank_control/tank_control.c) (Exercise 38)

### [Room Reservation System](41_rooms/rooms_with_list.cc) (Exercise 41)
`std::list`: inserting, erasing, sorting, traversing using iterator based loops

### [Count Specified Characters in Files / stdin](42_countchars/countchars.c) (Exercise 42)

### [Stock Trades Log](43_stocks/stocks.cc) (Exercise 43)
- used `std::map`, `std::deque`
- **NEVER** dereference `.end()`! $\rightarrow$ use `.front()` and `.back()` to access first/last element of a deque!

### [Scoreboard](44_scoreboard/scoreboard.c) (Exercise 44)
- line reading and parsing with `isblank()`, `isdigit()`, `isalnum()`
- `struct` with pointer to array of struct pointers
- `malloc()`/`realloc()`/`free()`
- sorting array of struct pointers according to a custom rule taking into account multiple struct members (in order to break ties)

### [Word Comparison](47_wcmp/wcmp.cc) (Exercise 47)
compares two sequences of words lexicographically: uses `std::vector` and `std::string` classes

### [Blob Data Structure](48_blob/blob.c) (Exercise 48)
manages a sequence of bytes stored in a doubly linked list with sentinel of chunks.

### [Line Length](49_linelen/linelen.c) (Exercise 49)

### [Video Serving Platform Statistics](50_videos/videos.c) (Exercise 50)

### [Text Expansion](52_expand/expand.cc) (Exercise 52)
expands patterns in a text according to a set of rules read from a file.
- rules are represented as two vectors of strings (patterns and their replacements)
- uses `std::string::substr()`, `std::string::find()` 

### [Snake Game](53_snake/snake.c) (Exercise 53)
simple game engine: board state, movement, growth, and collision handling.

### [Open Close Characters](54_openclose/openclose.c) (Exercise 54)

### [sequence of lines data structure](56_lines/words.cc) (Exercise 56)
manages a sequence of lines with add/remove/match operations  
- `new`/`delete` for object lifetime (dynamic allocation)
- `std::map<unsigned,std::string>` to map from ID $\rightarrow$ line, strictly monotonic `next_id` counter
- string parsing/tokenization via `isspace((unsigned char)c)`: `const char *` $\rightarrow$ `std::set<std::string>` (words in line)
- substring search with `.find()` + `std::string::npos`
- range-based `for` loops with `auto &`  
- pass-by-reference (`const auto &`) to avoid copies

### [String to Color](57_colors/colors.c) (Exercise 57)
computes RGB values from hex color code string

### [Text Styles](58_styles/styles_v2.cc) (Exercise 58)
parses style specs and resolves color names to RGB
- reads line-by-line with `while (std::getline(input_stream, line))`
- tokenizes each line into words with `std::stringstream` + `while (line_input >> word)`
- merges repeated `style` entries: `Style & style = styles[stylename]`

### [Alpha Code](59_alpha/alpha.cc) (Exercise 59)
"alpha" coding scheme: C-style, but uses an `Encoder` and `Decoder` class to encapsulate data and methods.

### [Rowing Club Management](60_rowing/rowing.cc) (Exercise 60)

### [Iterator](62_iterator/iterator_v2.c) (Exercise 62)

### [Big Letters](63_bigletters/bigletters.c) (Exercise 63)
ASCII big-letter renderer that parses a font file and prints enlarged text given as stdin input using a fancy font from the specified font file.

#### How to use:
```bash
./bigletters latexfont.txt <<< "LATEX"
```
Output:
```
.:***:.          X     .X*::XX::*X.              :*XX:  :XX: 
  *#:           :X*    ::   XX   ::                *#:  .*   
  *#.           :.#    :    XX    :                 *#. :    
  *#.          :..X*        XX      .*XX::::*X*      X#*     
  *#.         .:  .#.       XX        X#     .*      .#X     
  *#.        .*.  .X*.      XX        XX      :.     ::XX    
  *#.     ::                XX        X#   *        :. .#*   
  *#.    .X.                XX        X#::*X       ::   .#*  
.:X#*:::*XX               ::##::      XX   :   . :*#:   .##*:
                                      XX      .:
                                      X#      *.
                                    .*XX:::::XX
```


### [Output Checker](64_ochecker/ochecker.c) (Exercise 64)

### [Table Operations](midterm) (midterm 2024)
parse a text into items and reformat or compute values using placeholders and simple arithmetic expressions
- [tabops](midterm/tabops/tabops.c)
- [tabops2](midterm/tabops2/tabops2.c)


### [Compression Code](final) (final 2024)
encode and decode text files by replacing frequent words with single-byte codes
- [tdecode](final/tdecode/tdecode.c)
- [tencode in C (slow)](final/tencode/tencode.c)
- [tencode with a bit of C++ (very slow)](final/tencode_cpp_1/tencode.cpp): minimal C++ version (essentially just uses `std::vector` and `std::string` classes instead of manual `malloc`, `realloc` and `free` for dynamic arrays / strings)
- [tencode with C++ (fast)](final/tencode_cpp_2/tencode.cc): also uses `std::map` and `std::sort` for better performance
- [tencode with more C++](final/tencode_cpp_3/tencode.cc): uses `<fstream>` (`ifstream`/`ofstream`), stream ops `in.get`/`in.unget`, writes with `cout.put`/`cout.write`, `std::array<std::string,128>`, `std::vector`, `std::map`, `std::sort` + $\lambda$-function, pass-by-ref `ifstream&`, `using namespace std`
- [tencode with even more C++](final/tencode_cpp_4/tencode.cc):
  - `<sstream>` (`std::stringstream`) (no temp files/disk I/O) + generalized `istream&`
  - rewinds via `buffer.clear()` and `buffer.seekg(0, ios::beg)`
  - custom `get_word()` from a stream based on function (here: `isalpha`)



## Miscellaneous


### [Show Bit Pattern](show_bit_pattern/show_bits.c)
prints the bit pattern of integers.

#### How to use:
```bash
./show_bits 42
```
Output:
```
  _  _     _  _     _  _     _  _  
 | || |   | || |   | || |    _||_| 
 |_||_|   |_||_|   |_||_|   |_ | | 
□□□□□□□□ □□□□□□□□ □□□□□□□□ □□■□■□■□
                           ⁷      ⁰
```


### [Print Number Recursively](print_number_recursively/print_number_recursively.c)
prints an integer using recursion.


### [Hashtable](book-exercises/exercise6-5-hashtable.c) (K&R 6–5)
string hashtable that stores key-value pairs (both strings) with functions to add, retrieve, and remove entries.


## [C++ Introduction](cpp_introduction)

### Examples

- [Dynamic Array in C](cpp_introduction/01_dynamic_array.c): dynamic array of integers using `malloc`, `realloc`, `free`, etc (shows how much work needs to be done in C to get a very basic data structure that can grow/shrink as needed).
- [Dynamic Array in C++](cpp_introduction/02_vector_int.cpp): same purpose as above, but using C++ `std::vector<int>` class (much simpler and less error-prone). also uses `auto`, iterator-based `for` loop, range-based `for` loop, `std::cin`, `std::cout`, `std::endl`.
- [using a C array in C++](cpp_introduction/03_C_array_float.cpp): shows how to apply C++ features on a raw C array ($\rightarrow$ we can pass the begin/end pointers of the C array to `std::sort`, where the end pointer is one-past-the-end and can be computed as `A + n` where `n` is the number of elements in the array).
- [Graph (C++)](cpp_introduction/04_graph.cpp): reading a graph from a file, storing it as an adjacency list, and performing a BFS. shows how to use `vector <vector <unsigned> >`, `map` ($\rightarrow$ careful with `value = M[key]` (if `key` is not found, it creates a new entry in the map!), `pair` container), file/string-streams (`fstream`/`sstream`), (double-ended) queue (`deque`) and how to push/pop from it, reading lines from a stream into a string (`getline`), how to ignore empty lines, initialize a vector of a given size.

### [Notes on References](cpp_introduction/references.md)







