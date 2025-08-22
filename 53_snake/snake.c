#include <stdio.h>
#include <stdlib.h>

// snake directions
#define SNAKE_UP 1
#define SNAKE_DOWN 2
#define SNAKE_LEFT 3
#define SNAKE_RIGHT 4

// game status
#define SNAKE_OKAY 1
#define SNAKE_FAIL 2
#define SNAKE_NEW 3

struct point {
    int c;
    int r;
};

struct snake {
    int cols;
    int rows;

    char * board; //  idx = r * cols + c  <=>  r = idx / cols, c = idx % rows

    int len;
    
    int maxlength;
    struct point * body;
    int head; // idx for body array
    int tail; // idx for body array

    int status;
    int dir;
};

struct snake * snake_new(int c, int r, int l) {
    struct snake * s = (struct snake *) realloc(NULL, sizeof(struct snake));
    s->board = (char *) realloc(NULL, c * r * sizeof(char) + 1); // space for terminating null character
    s->board[c * r] = '\0';
    s->cols = c;
    s->rows = r;
    s->status = SNAKE_NEW;
    s->maxlength = l;
    return s;
}


void snake_destroy(struct snake * s) {
    free(s->board);
    free(s->body);
    free(s);
}

int head(struct snake * s) {
    return s->body[s->head].r * s->cols + s->body[s->head].c; // head idx inside board string
}
int tail(struct snake * s) {
    return s->body[s->tail].r * s->cols + s->body[s->tail].c; // tail idx inside board string
}

int snake_start (struct snake * s, int c, int r, int d) {
    if ((0 <= c && c < s->cols) && (0 <= r && r < s->rows) && (d == SNAKE_UP || d == SNAKE_DOWN || d == SNAKE_LEFT || d == SNAKE_RIGHT)) {
        s->body = (struct point *) realloc(NULL, s->maxlength * sizeof(struct point));
        s->head = 0;
        s->tail = 0;
        s->body[s->head].c = c;
        s->body[s->head].r = r; 
        s->len = 1;
        s->dir = d;
        for (int i = 0; i < s->rows * s->cols; i++) {
            (s->board)[i] = (i == head(s)) ? '@' : ' ';
        }
        s->status = SNAKE_OKAY;
        return SNAKE_OKAY;
    } else {
        s->status = SNAKE_FAIL;
        return SNAKE_FAIL;
    }
}

int snake_change_direction (struct snake * s, int d) {
    if (d == SNAKE_UP || d == SNAKE_DOWN || d == SNAKE_LEFT || d == SNAKE_RIGHT) {
        s->dir = d;
        return s->status;
    } 
    return SNAKE_FAIL;
}

int snake_step (struct snake * s) {
    if (s->status == SNAKE_OKAY) {
        int old_head_idx = head(s);
        int old_tail_idx = tail(s);

        int r_head_before = s->body[s->head].r;   
        int c_head_before = s->body[s->head].c;   

        int r_head_after = r_head_before;    
        int c_head_after = c_head_before;

        switch (s->dir) {
            case SNAKE_UP: r_head_after = r_head_before - 1; break;
            case SNAKE_DOWN: r_head_after = r_head_before + 1; break; 
            case SNAKE_LEFT: c_head_after = c_head_before - 1; break;
            case SNAKE_RIGHT: c_head_after = c_head_before + 1; break;
        }
        if  (      (0 <= c_head_after && c_head_after < s->cols) 
                && (0 <= r_head_after && r_head_after < s->rows) 
                && (        (s->board)[r_head_after * s->cols + c_head_after] == ' ' 
                        || ((s->board)[r_head_after * s->cols + c_head_after] == '+' && s->len == s->maxlength))
            ) {
            s->head = (s->head + 1) % s->maxlength;
            if (s->len < s->maxlength) {
                s->len++;
            } else {
                s->tail = (s->tail + 1) % (s->maxlength);
            }
            s->body[s->head].c = c_head_after;
            s->body[s->head].r = r_head_after;
            s->board[old_head_idx] = '#';
            s->board[old_tail_idx] = ' ';
            s->board[tail(s)] = '+';
            s->board[head(s)] = '@';
            return SNAKE_OKAY;
        } else {
            s->status = SNAKE_FAIL;
        }
    }
    return SNAKE_FAIL;
}

const char * snake_get_field (const struct snake * s) {
    return s->board;
}

int snake_get_status (const struct snake * s) {
    return s->status;
}