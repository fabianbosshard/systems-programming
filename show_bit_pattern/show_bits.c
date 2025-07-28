#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

static char *super[] = {"\xe2\x81\xb0", "\xc2\xb9", "\xc2\xb2", "\xc2\xb3", "\xe2\x81\xb4", "\xe2\x81\xb5", "\xe2\x81\xb6", "\xe2\x81\xb7", "\xe2\x81\xb8", "\xe2\x81\xb9"};
static char *sub[] = {"\xe2\x82\x80", "\xe2\x82\x81", "\xe2\x82\x82", "\xe2\x82\x83", "\xe2\x82\x84", "\xe2\x82\x85", "\xe2\x82\x86", "\xe2\x82\x87", "\xe2\x82\x88", "\xe2\x82\x89"};


int get_number() {
    char * buf = malloc(sizeof(char));
    int i;
    for (i = 0; (buf[i++] = getchar()) != '\n'; buf = realloc(buf, (i + 1) * sizeof(char)));
    buf[--i] = '\0';
    int n = atoi(buf);
    free(buf);
    return n;
}

static void print_hex_ascii(int n) {
    const int bytes           = sizeof n;     // e.g. 4
    const int H               = 3;            // height of our hex art
    const int W_CHAR          = 3;            // width per hex digit
    const int N_DIGITS        = 2;            // two hex digits per byte
    const int ART_W           = W_CHAR * N_DIGITS;      // 6 columns per byte
    const int BITS_W          = CHAR_BIT;              // 8 bits per byte
    const int SEP             = 1;                     // space between byte-groups
    const int PAD_LEFT        = (BITS_W - ART_W) / 2;  // = (8−6)/2 = 1
    const int PAD_RIGHT       = BITS_W - ART_W - PAD_LEFT; // also 1

    // 3×3 patterns for 0…F
    static const char *hex_art[16][3] = {
        /*0*/ {" _ ","| |","|_|"},
        /*1*/ {"   ","  |","  |"},
        /*2*/ {" _ "," _|","|_ "},
        /*3*/ {" _ "," _|"," _|"},
        /*4*/ {"   ","|_|","  |"},
        /*5*/ {" _ ","|_ "," _|"},
        /*6*/ {" _ ","|_ ","|_|"},
        /*7*/ {" _ ","  |","  |"},
        /*8*/ {" _ ","|_|","|_|"},
        /*9*/ {" _ ","|_|"," _|"},
        /*A*/ {" _ ","|_|","| |"},
        /*b*/ {"   ","|_ ","|_|"},
        /*C*/ {" _ ","|  ","|_ "},
        /*d*/ {"   "," _|","|_|"},
        /*E*/ {" _ ","|_ ","|_ "},
        /*F*/ {" _ ","|_ ","|  "}
    };

    for (int row = 0; row < H; row++) {
        for (int byte_i = 0; byte_i < bytes; byte_i++) {
            // 1) left‐pad so this 6-col block sits centered over the 8 bits
            for (int s = 0; s < PAD_LEFT; s++) putchar(' ');

            // 2) pick out MSB‐first
            int shift = 8 * (bytes - 1 - byte_i);
            unsigned char byte = (n >> shift) & 0xFF;
            int hi = (byte >> 4) & 0xF;
            int lo = byte & 0xF;

            // 3) print the two 3-char slices
            printf("%s%s", hex_art[hi][row], hex_art[lo][row]);

            // 4) right‐pad within this 8-bit group
            for (int s = 0; s < PAD_RIGHT; s++) putchar(' ');

            // 5) the single space between bytes
            if (byte_i + 1 < bytes) putchar(' ');
        }
        putchar('\n');
    }
}

int main(int argc, char * argv[]) {
    int n;

    if (argc == 1) {
        start:
        printf("please input the number and hit return:  ");
        fflush(stdin);
        n = get_number();
    } else if (argc == 2) {
        n = atoi(argv[1]);
    } else {
        printf("too many arguments, input number and hit return:  ");
        n = get_number();
    }

    print_hex_ascii(n);
    for (int j = 0; j < 8 * sizeof(n); j++) {
        if (j > 0 && j % 8 == 0) putchar(' ');
        if ((((1u << (8 * sizeof(n) - 1)) >> j) & n) != 0) {
            printf("\xE2\x96\xA0");
        } else {
            printf("\xE2\x96\xA1");
        }
    }
    putchar('\n');
    for (int j = 8 * sizeof(n) - 1; j >= 0; j--) {
        if (j > 0 && j % 8 == 0) putchar(' ');
        if (j == 0 || j == 7) {
            printf("%s",super[j]);
        } else {
            putchar(' ');
        }
    }
    putchar('\n');
    putchar('\n');
    goto start;
}