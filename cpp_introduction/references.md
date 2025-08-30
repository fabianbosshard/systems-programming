
### References

a reference is **not** an object, it is an alias for an object (another name for the same object).
in simple cases, the compiler might not even allocate any memory, and (conceptually) just replace every use of `ref` with `n`.
in other cases, the compiler needs to store a reference, in which case the underlying implementation will probably consist of a pointer object.
```cpp
int n = 42;
int & ref = n;
```

main use is to pass as parameter as well as return from a function (there is a swap-function in the standard library, we do not need to define it ourselves, it is just an example):
```cpp
/* C style */
void swap(int * p, int * q) {
  int tmp = *p;
  *p = *q;
  *q = tmp;
}
/* C++ style */
void swap(int & a, int & b) {
  int tmp = a;
  a = b;
  b = tmp;
}
```
in C++ it is also possible to overload functions, so depending on the type passed passed, one or the other `swap` function will be invoked.
ironically (but consistently), invoking with the `&` (address-of) operator, the C style function will be invoked.

for big objects, it makes sense to *pass by reference*, because copying a big object would be inefficient. if we do not want the function to change the object, we can pass by `const` reference:
```cpp
/* inefficient */
void print_vec(std::vector<int> V) {
    for (auto itr = V.begin(); itr != V.end(); itr++) {
        std::cout << *itr << std::endl;
    }
}
```
```cpp
/* more efficient */
void print_vec(const std::vector<int> & V) {
    for (auto itr = V.begin(); itr != V.end(); itr++) {
        std::cout << *itr << std::endl;
    }
}
```

the beautiful (and necessary) benefit of references is not so much in expressions that are function calls but in the use of user-defined operators (see the [matrix example](06_operator_overloading.cpp)).

