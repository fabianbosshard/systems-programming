#include <stdio.h>
#include <limits.h>
#include <float.h>


// https://stackoverflow.com/questions/597813/how-to-print-subscripts-superscripts-on-a-cli
static char *super[] = {"\xe2\x81\xb0", "\xc2\xb9", "\xc2\xb2", "\xc2\xb3", "\xe2\x81\xb4", "\xe2\x81\xb5", "\xe2\x81\xb6", "\xe2\x81\xb7", "\xe2\x81\xb8", "\xe2\x81\xb9"};
static char *sub[] = {"\xe2\x82\x80", "\xe2\x82\x81", "\xe2\x82\x82", "\xe2\x82\x83", "\xe2\x82\x84", "\xe2\x82\x85", "\xe2\x82\x86", "\xe2\x82\x87", "\xe2\x82\x88", "\xe2\x82\x89"};


void print_binary_representation(int n) {
    for (unsigned int mask = 1u << (sizeof(unsigned int) * 8 - 1); mask != 0; mask = mask >> 1) {
        if ((mask & n) != 0) {
            putchar('1');
        } else {
            putchar('0');
        }
    } 
    putchar('\n');
}


int main(int argc, char * argv[]) {
    int a = 5;
    int b = -5;
    printf("binary representation of a and b:\n");
    print_binary_representation(a);
    print_binary_representation(b);

    putchar('\n');

    printf("hex representation of a and b:\n");
    printf("\ta = (%d)%s%s = (%x)%s%s\n", a, sub[1], sub[0], a, sub[1], sub[6]);
    printf("\tb = (%d)%s%s = (%x)%s%s\n", b, sub[1], sub[0], b, sub[1], sub[6]);


    printf("%f",1e-2);
}