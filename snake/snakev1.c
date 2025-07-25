#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define EMPTY 0
#define SNAKE 1
#define APPLE 2


void print_board(int * board, int row_num, int col_num) {
    for (int i = 0; i < row_num; i++) {
        for (int j = 0; j < col_num; j++) {
            switch (board[i*col_num+j]) {
                case EMPTY: putchar(' '); break;
                case SNAKE: putchar('+'); break;
                case APPLE: putchar('@'); break;
            }
        }
        putchar('\n');
    }
}

int main(int argc, char * argv[]) {
    int board[5][5];
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            board[i][j] = EMPTY;
        }
    }
    board[2][1] = SNAKE;
    board[2][2] = SNAKE;
    board[3][2] = SNAKE;
    board[3][3] = SNAKE;
    board[3][4] = SNAKE;
    print_board(board,5,5);
}