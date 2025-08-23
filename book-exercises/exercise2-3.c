#include <stdio.h>


int hexvalue(char c) {
    int n;
    if (c >= '0' && c <= '9') {
        n = c - '0';
    }
    if (c >= 'a' && c <= 'f') {
        n = c - 'a' + 10;
    }
    if (c >= 'A' && c <= 'F') {
        n = c - 'A' + 10;
    }
    return n;
}


int htoi(char s[]) {
    int n = 0;
    for (int i = 0; s[i] != '\0'; ++i) {
        n = 16 * n + hexvalue(s[i]);
    }
    return n;
}

int main(int argc, char * argv[]) {

    printf("%d\n",htoi("1f"));

}