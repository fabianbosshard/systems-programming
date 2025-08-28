#include <iostream>
#include <vector>



/* 
istream class does overloading of `<<`:
istream & operator >> (istream & input, int & var);
*/



class Matrix { 
    
    // by default everything is private
    int * D;
    unsigned n_rows;
    unsigned n_cols;
    
    public:

    Matrix(unsigned r_, unsigned c_) {
        n_rows = r_;
        n_cols = c_;
        D = (int *) malloc(sizeof(int) * n_rows * n_cols); // in C++ we need to cast it...
    }

    // but how do we initialize? in C / Java we would have a 'setter' method... in C++ we have a more elegant solution:
    int & operator()(unsigned i, unsigned j) {  // overloading the function call operator
        return D[n_cols * i + j];
    } // when we return a reference, the thing returned is an l-value... so now we can use methods on the left hand side of an assignment (beautiful!)...
    // can be used to get as well as set the value :) 

    void clear() {
        for (unsigned i = 0; i < n_rows; i++) {
            for (unsigned j = 0; j < n_cols; j++) {
                D[i * n_cols + j] = 0;
            }
        }
    }

    void print() {
        for (unsigned i = 0; i < n_rows; i++) {
            for (unsigned j = 0; j < n_cols; j++) {
                std::cout << D[i * n_cols + j] << " ";
            }
            std::cout << std::endl;
        }
    }


};

int main() {
    int x;
    std::vector<int> A;
    while (std::cin >> x) { // in that case the istream class overloads the operator
        A.push_back(x);
    }


    Matrix M(7, 13);
    M.clear();
    M(1,2) = 7;
    M.print();

}