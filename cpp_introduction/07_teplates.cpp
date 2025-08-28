#include <iostream>
#include <vector>

/* 
templates:
ways to define families of classes, structs, functions, and more (as always in C++)

a family is a set of functions that are variants of the same function that are parameterized by a type

but we use templates (from the standard library) much more than we define our own...
*/


template <typename T> void swap (T * p, T * q) { // this is not just one function, but a family of functions...
    T tmp = *p;
    *p = *q;
    *q = tmp;
}


template <typename T> struct mylist {
    T value;
    mylist * prev;
    mylist * next;
};



int main () {


    char buf [] = { 'c', 'i', 'a', 'o', 0};
    swap(buf, buf+3);
    std::cout << buf << std::endl;



    mylist<int> L_i;
    mylist<float> L_f;


}
