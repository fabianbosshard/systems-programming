#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

class Encoder {
private:
    FILE * input;
    FILE * output;  
    void emit(int c) {
        putc(c, output); 
        emitted++;
        if (emitted % 80 == 0) {
            putc('\n', output);
        }
    }

public:
    int emitted;
    void encode() {
        int c;
        while ((c = getc(input)) != EOF) {
            if (isalpha(c)) {
                if (c == 'Q') {
                    emit('Q');
                    emit('Q');
                } else {
                    emit(c);
                }
            } else {
                emit('Q');
                emit('a' + c / 16);
                emit('a' + c % 16);
            }
        }
        if (emitted % 80 != 0) putc('\n', output);
    }
    Encoder(FILE * in, FILE * out) {
        emitted = 0;
        input = in;
        output = out;
    }
};


class Decoder {
private:
    FILE * input;
    FILE * output;
    int get_byte() {
        int c1;
        while (!isalpha(c1 = getc(input))) {
            if (c1 == EOF) exit(EXIT_FAILURE);
        }
        if (c1 == 'Q') {
            return 'Q';
        }
        if (c1 - 'a' < 0 || c1 - 'a' >= 16) exit(EXIT_FAILURE);

        int c2;
        while (!isalpha(c2 = getc(input))) {
            if (c2 == EOF) exit(EXIT_FAILURE);
        }
        if (c2 - 'a' < 0 || c2 - 'a' >= 16) exit(EXIT_FAILURE);

        int b = (c1 - 'a') * 16 + (c2 - 'a');
        return b;
    }

public:
    void decode() {
        int c;
        while ((c = getc(input)) != EOF) {
            if (isalpha(c)) {
                if (c == 'Q') {
                    int b = get_byte();
                    putc(b, output);
                } else {
                    putc(c, output);
                }
            }
        }
    }
    Decoder(FILE * in, FILE * out) {
        input = in;
        output = out;
    }
};


int main(int argc, char * argv[]) {
    if (argc == 1) {
        Encoder e(stdin, stdout);
        e.encode();
        return EXIT_SUCCESS;
    } else if (argc == 2 && strcmp(argv[1], "-d") == 0) {
        Decoder d(stdin, stdout);
        d.decode();
        return EXIT_SUCCESS;
    } else {
        exit(EXIT_FAILURE);
    }
}