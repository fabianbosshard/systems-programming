#include <stdio.h>
#include <stdlib.h>





void print_number(int num) {
    if (num == 0) {
        return;
    }
    print_number(num / 10);
    putchar('0' + num % 10);
}

int main(int argc, char * argv[]) {


    int number = 13424;

    print_number(number);

}