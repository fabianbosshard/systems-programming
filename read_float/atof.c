#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

double my_atof(char * s) { // ASCII to float
    double result = 0;
    int i = 0;
    int sign = 1;
    for (; isspace(s[i]); i++);
    if (s[i] == '+' || s[i] == '-') {
        sign = (s[i] == '-') ? -1 : 1;
        i++;
    }
    for (; isdigit(s[i]); i++) {
        result = 10 * result + (s[i] - '0');
    }
    if (s[i] != '.') {
        return sign * result;
    }
    i++;
    for (double divisor = 10; isdigit(s[i]); divisor *= 10, i++) {
        result = result + (s[i] - '0') / divisor;
    }
    return sign * result;
}