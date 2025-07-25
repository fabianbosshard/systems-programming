#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // write(), usleep()
#include <termios.h> // raw‑mode keyboard (no Enter needed)
#include <sys/select.h> // kbhit()‑style polling

#define WIDTH  40
#define HEIGHT 20
#define INITIAL_LEN 5
#define TICK_USEC 100000  // microseconds per frame

typedef struct { int r, c; } Point;

enum { UP=0, DOWN, LEFT, RIGHT };

static struct termios orig_term; // terminal settings backup
void restore_terminal() {tcsetattr(STDIN_FILENO, TCSANOW, &orig_term);} // restore terminal to original settings
void setup_terminal() { // set terminal to raw mode (non-canonical, no echo)
    struct termios raw;
    tcgetattr(STDIN_FILENO, &orig_term);
    raw = orig_term;
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    atexit(restore_terminal);
}
int kbhit() { // check if a key was pressed
    struct timeval tv = {0, 0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO+1, &fds, NULL, NULL, &tv) > 0;
}
void clear_screen() {write(STDOUT_FILENO, "\033[H\033[J", 6);} // clear screen and move cursor home

void print_board(char board[HEIGHT][WIDTH], int score) {
    clear_screen();
    for (int j = 0; j < WIDTH+2; j++) putchar('#'); // top border
    putchar('\n');
    for (int i = 0; i < HEIGHT; i++) {
        putchar('#'); // left boarder
        for (int j = 0; j < WIDTH; j++) {
            putchar(board[i][j]); // inner positions
        }
        putchar('#'); // right border
        putchar('\n');
    }
    for (int c = 0; c < WIDTH+2; c++) putchar('#'); // bottom border
    putchar('\n');
    printf("Score: %d\n", score);
}


int main() {
    char board[HEIGHT][WIDTH];
    Point snake[WIDTH*HEIGHT];
    int dir = RIGHT;
    int score = 0;
    int head = 0, tail = 0, length = INITIAL_LEN;

    srand(time(NULL));
    setup_terminal();

    // init board empty
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            board[i][j] = ' ';

    // init snake in middle
    int midr = HEIGHT/2, midc = WIDTH/2;
    for (int n = 0; n < INITIAL_LEN; n++) {
        snake[n].r = midr;
        snake[n].c = midc - (INITIAL_LEN-1) + n;
        board[midr][midc - (INITIAL_LEN-1) + n] = '+';
    }
    head = INITIAL_LEN - 1;

    // place initial apple
    Point apple;
    while (1) {
        int ar = rand()%HEIGHT, ac = rand()%WIDTH;
        if (board[ar][ac] == ' ') { apple.r = ar; apple.c = ac; board[ar][ac] = '@'; break; }
    }

    // game loop
    while (1) {
        usleep(TICK_USEC);
        // input
        if (kbhit()) {
            char ch = getchar();
            if (ch == 'q') break;
            if ((ch=='w' || ch=='W') && dir!=DOWN)    dir = UP;
            if ((ch=='s' || ch=='S') && dir!=UP)      dir = DOWN;
            if ((ch=='a' || ch=='A') && dir!=RIGHT)   dir = LEFT;
            if ((ch=='d' || ch=='D') && dir!=LEFT)    dir = RIGHT;
        }
        // compute new head
        Point cur = snake[head];
        Point nxt = cur;
        if      (dir==UP)    nxt.r--;
        else if (dir==DOWN)  nxt.r++;
        else if (dir==LEFT)  nxt.c--;
        else if (dir==RIGHT) nxt.c++;
        // check collisions
        if (nxt.r<0||nxt.r>=HEIGHT||nxt.c<0||nxt.c>=WIDTH) break;
        if (board[nxt.r][nxt.c] == '+') break;
        // move head
        head = (head + 1) % (WIDTH*HEIGHT);
        snake[head] = nxt;
        int ate = (board[nxt.r][nxt.c]=='@');
        board[nxt.r][nxt.c] = '+';
        if (ate) {
            score++;
            length++;
            // place new apple
            while (1) {
                int ar = rand()%HEIGHT, ac = rand()%WIDTH;
                if (board[ar][ac] == ' ') { apple.r = ar; apple.c = ac; board[ar][ac] = '@'; break; }
            }
        } else {
            // remove tail
            Point t = snake[tail];
            board[t.r][t.c] = ' ';
            tail = (tail + 1) % (WIDTH*HEIGHT);
        }
        print_board(board, score);
    }

    restore_terminal();
    clear_screen();
    printf("Game Over! Final Score: %d\n", score);
    return 0;
}