#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    char * board;

    int maxlength;

    //    addr = r * cols + c         |        r = addr / cols,     c = addr % rows
    struct point * body;
    int len;

    int status;
    int dir;
};

struct snake * snake_new(int c, int r, int l) {
    struct snake * s = realloc(NULL, sizeof(struct snake));
    s->board = realloc(NULL, c * r * sizeof(char) + 1); // space for terminating null character
    s->board[c * r] = '\0';
    s->body = NULL;
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
    return s->body[0].r * s->cols + s->body[0].c;
}
int limb(struct snake * s, int nr) {
    return s->body[nr].r * s->cols + s->body[nr].c;
}
int tail(struct snake * s) {
    return s->body[s->len - 1].r * s->cols + s->body[s->len - 1].c;
}

int snake_start (struct snake * s, int c, int r, int d) {
    if ((0 <= c && c < s->cols) && (0 <= r && r < s->rows) && (d == SNAKE_UP || d == SNAKE_DOWN || d == SNAKE_LEFT || d == SNAKE_RIGHT)){
        s->body = realloc(s->body, sizeof(struct point));
        s->body[0].c = c;
        s->body[0].r = r; 
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
    return SNAKE_OKAY;
}


int snake_step (struct snake * s) {
    if (s->status == SNAKE_OKAY) {
        int r_head_before = s->body[0].r;   
        int c_head_before = s->body[0].c;   
        int r_head_after = r_head_before;    
        int c_head_after = c_head_before;

        int old_head_idx = head(s);
        int old_tail_idx = tail(s);

        switch (s->dir) {
            case SNAKE_UP: r_head_after = r_head_before - 1; break;
            case SNAKE_DOWN: r_head_after = r_head_before + 1; break; 
            case SNAKE_LEFT: c_head_after = c_head_before - 1; break;
            case SNAKE_RIGHT: c_head_after = c_head_before + 1; break;
        }
        if (
                   (0 <= c_head_after && c_head_after < s->cols) 
                && (0 <= r_head_after && r_head_after < s->rows) 
                && (    
                            (s->board)[r_head_after * s->cols + c_head_after] == ' ' 
                        || ((s->board)[r_head_after * s->cols + c_head_after] == '+' && s->len == s->maxlength))
        ) {
            if (s->len < s->maxlength) {
                s->body = realloc(s->body, (++(s->len)) * sizeof(struct point));
            }
            for (int k = s->len - 1; k > 0; k--) {
                s->body[k] = s->body[k - 1];
            }
            s->body[0].c = c_head_after;
            s->body[0].r = r_head_after;
            if (s->len > 2) {
                s->board[old_head_idx] = '#';
            }
            if (s->len == s->maxlength) {
                s->board[old_tail_idx] = ' ';
            }
            (s->board)[tail(s)] = '+';
            (s->board)[head(s)] = '@';

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
