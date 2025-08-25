# Preparation for Systems Programming Retake


## [Binary Expressions](02_binary_expressions/bexp.c)
Binary/bit manipulation utilities and expression evaluation.


## [Snake Game](53_snake/snake.c)
simple game engine: board state, movement, growth, and collision handling.


## [Big Letters](63_bigletters/bigletters.c)
ASCII big-letter renderer that parses a font file and prints enlarged text given as stdin input using a fancy font from the specified font file.

### How to use:
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


## [Show Bit Pattern](show_bit_pattern/show_bits.c)
prints the bit pattern of integers.

### How to use:
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



## [Print Number Recursively](print_number_recursively/print_number_recursively.c)
prints an integer using recursion.


## [Table Operations](midterm) (midterm 2024)
parse a text into items and reformat or compute values using placeholders and simple arithmetic expressions
- [tabops](midterm/tabops/tabops.c)
- [tabops2](midterm/tabops2/tabops2.c)


## [Compression Code](final) (final 2024)
encode and decode text files by replacing frequent words with single-byte codes
- [tdecode](final/tdecode/tdecode.c)
- [tencode in C (slow)](final/tencode/tencode.c)
- [tencode in C++ (very slow)](final/tencode_cpp_butjustalittle/tencode.cpp): minimal C++ version (essentially just uses std::vector and std::string classes instead of manual malloc, realloc and free for dynamic arrays / strings)
- [tencode in C++ (fast)](final/tencode_cpp/tencode.cc): also uses std::map and std::sort for better performance



## [Hashtable](book-exercises/exercise6-5-hashtable.c) (K&R 6–5)
string hashtable that stores key-value pairs (both strings) with functions to add, retrieve, and remove entries.