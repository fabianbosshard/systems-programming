#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector <int> A; // template: data structure parameterized by the type of the objects it contains
    int x;
    while (std::cin >> x) { // iterate as long as there are numbers in the input to read: true if operation succeeds, false if operation fails
        A.push_back(x);
    }
    std::sort(A.begin(), A.end()); // takes 2 `iterators`. an iterator is a pointer-like object. end() in this case points one past the last element.
    
    /* output: */ 
    // easiest
    for (int i = 0; i < A.size(); i++) {
        std::cout << A[i] << std::endl; 
    }
    // iterator of vector of integers
    for (std::vector<int>::const_iterator i = A.begin(); i != A.end(); i++) {  // implementation of `i++` depends on the type of the data structure (e.g. in case of list it does not increment anything but rather follows the pointer to the next object `ptr = ptr->next`)
        std::cout << *i << std::endl;  // dereferencing the iterator: gives the value
    }
    // using `auto`
    for (auto i = A.begin(); i != A.end(); i++) {  // C++ is smart enough to figure out the correct type for `i`, since it is initialized with an initializer expression and it knows what the type of the expression is
        std::cout << *i << std::endl;
    }
    // range-based for loop (sometimes called "range-for loop"): iterates over every element in a container, automatically handling the begin-to-end iteration
    for (int x : A) {
        std::cout << x << std::endl; 
    }
    // also here we can use `auto`
    for (auto x : A) {
        std::cout << x << std::endl; 
    }
}