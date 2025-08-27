#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    double A[1000];
    double x;
    unsigned n;
    for (n = 0; n < 1000 && (std::cin >> x); n++) { // checking both that we are not going past the size of the array and that we actualy have a number to read
        A[n] = x;
    }
    std::sort(A, A + n); // std::sort is written to work with random-access iterators — and in C++, a raw pointer (like double*) is also a random-access iterator
    /*
    Why a pointer works as an iterator:
    In C++, pointers support all the operations that random-access iterators need:
        •	*ptr → dereference to access the value
        •	ptr + n or ptr - n → arithmetic to jump positions
        •	ptr1 < ptr2 → comparison between pointers in the same array
        •	++ptr or --ptr → increment/decrement
        •	ptr2 - ptr1 → distance between two elements
    This means you can treat the array range [A, A + n) exactly like std::vector<double>::begin() and std::vector<double>::end().
    */
    for (auto x : A) {
        std::cout << x << std::endl; 
    }
}