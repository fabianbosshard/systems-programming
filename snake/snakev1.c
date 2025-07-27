#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // write(), usleep()
#include <termios.h> // raw‑mode keyboard (no Enter needed)
#include <sys/select.h> // kbhit()‑style polling


#define APPLE 0
#define SNAKE 1
#define EMPTY 2

#define BG_RED     "\x1b[41m"   // red background
#define BG_BLUE    "\x1b[44m"   // blue background
#define BG_GRAY    "\x1b[100m"  // bright-black (gray) background
#define ANSI_RESET "\x1b[0m"

#define WIDTH  40
#define HEIGHT 40
#define INITIAL_LEN 5
#define TICK_USEC 100000  // microseconds per frame

typedef struct { int r, c; } Point;

enum { UP, DOWN, LEFT, RIGHT };


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
    for (int j = 0; j < WIDTH+2; j++) printf(BG_GRAY "  " ANSI_RESET); // top border
    putchar('\n');
    for (int i = 0; i < HEIGHT; i++) {
        printf(BG_GRAY "  " ANSI_RESET); // left boarder
        for (int j = 0; j < WIDTH; j++) {
            switch (board[i][j]) { // inner positions
                case EMPTY: printf("  "); break;
                case SNAKE: printf(BG_BLUE "  " ANSI_RESET); break;
                case APPLE: printf(BG_RED  "  " ANSI_RESET); break;
            }
        }
        printf(BG_GRAY "  " ANSI_RESET); putchar('\n'); // right border
    }
    for (int c = 0; c < WIDTH+2; c++) printf(BG_GRAY "  " ANSI_RESET); // bottom border
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
            board[i][j] = EMPTY;

    // init snake in middle
    int midr = HEIGHT/2, midc = WIDTH/2;
    for (int n = 0; n < INITIAL_LEN; n++) {
        snake[n].r = midr;
        snake[n].c = midc - (INITIAL_LEN-1) + n;
        board[midr][midc - (INITIAL_LEN-1) + n] = SNAKE;
    }
    head = INITIAL_LEN - 1;

    // place initial apple
    Point apple;
    while (1) {
        int ar = rand()%HEIGHT, ac = rand()%WIDTH;
        if (board[ar][ac] == EMPTY) { apple.r = ar; apple.c = ac; board[ar][ac] = APPLE; break; }
    }

    // 1) draw the empty board with snake + apple
    print_board(board, score);

    // 2) block until the user hits a key
    while (!kbhit()) {
        usleep(TICK_USEC);
    }
    char ch = getchar();
    // 3a) if they hit 'q', quit
    if (ch == 'q' || ch == 'Q') {
        restore_terminal();
        clear_screen();
        printf("Bye\n");
        return 0;
    }
    // 3b) otherwise treat it as an initial direction
    if ((ch=='w' || ch=='W') && dir!=DOWN)    dir = UP;
    else if ((ch=='s' || ch=='S') && dir!=UP) dir = DOWN;
    else if ((ch=='a' || ch=='A') && dir!=RIGHT)  dir = LEFT;
    else if ((ch=='d' || ch=='D') && dir!=LEFT)   dir = RIGHT;

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
        if (board[nxt.r][nxt.c] == SNAKE) break;
        // move head
        head = (head + 1) % (WIDTH*HEIGHT);
        snake[head] = nxt;
        int ate = (board[nxt.r][nxt.c]==APPLE);
        board[nxt.r][nxt.c] = SNAKE;
        if (ate) {
            score++;
            length++;
            // place new apple
            while (1) {
                int ar = rand()%HEIGHT, ac = rand()%WIDTH;
                if (board[ar][ac] == EMPTY) { apple.r = ar; apple.c = ac; board[ar][ac] = APPLE; break; }
            }
        } else {
            // remove tail
            Point t = snake[tail];
            board[t.r][t.c] = EMPTY;
            tail = (tail + 1) % (WIDTH*HEIGHT);
        }
        print_board(board, score);
    }

    restore_terminal();
    clear_screen();
    printf("Game Over! Final Score: %d\n", score);
    return 0;
}