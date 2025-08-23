/* copy input to output:

int c;

while ((c = getchar()) != EOF)
    putchar(c);

*/




/* V1
#include <stdio.h>


int main()
{
    int c;

    
    // exercise 1-9
    while ((c = getchar()) != EOF) {
        if (c == ' ') {
            putchar(' ');
            while ((c = getchar()) == ' '); // in C, an assignment (such as "c = getchar()")is also an expression and has a value, which is the value of the left hand side after the assignment
            if (c == EOF)                   // reached end of file while skipping
                break;
            else
                putchar(c);
        } else {
            putchar(c);
        }
    }
}

*/ 

#include <stdio.h>

int main()
{
    int c;

    // exercise 1-9, v2 (less reduntant more compact)
    while ((c = getchar()) != EOF) {
        if (c == ' ') {
            putchar(' ');
            while ((c = getchar()) == ' ') {;}
            if (c == EOF) break;
        }
        putchar(c);
    }
}