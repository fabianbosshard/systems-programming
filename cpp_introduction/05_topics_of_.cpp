#include <iostream>
#include <vector>
#include <string>



// /* inefficient */
// void print_vec(std::vector<int> V) {
//     for (auto itr = V.begin(); itr != V.end(); itr++) {
//         std::cout << *itr << std::endl;
//     }
// }

/* more efficient */
void print_vec(const std::vector<int> & V) { // make const
    for (auto itr = V.begin(); itr != V.end(); itr++) {
        std::cout << *itr << std::endl;
    }
}


int main() {
    std::vector<int> vec = {1,2,3,4};
    print_vec(vec);
}