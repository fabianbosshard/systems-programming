#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


struct team {
    char * name;
    int points;
    int games;
};

struct scoreboard {
    int num_teams;
    struct team * * teams; // array of pointers to team structs
    int max_num_teams;
};


struct scoreboard * new_board(int N) {
    struct scoreboard * b = malloc(sizeof(struct scoreboard));
    b->max_num_teams = N;
    b->num_teams = 0;
    b->teams = calloc(N, sizeof(struct team *));
    return b;
}

struct team * find_team(struct scoreboard * b, char * name) {
    for (int i = 0; i < b->num_teams; i++) {
        if(strcmp(b->teams[i]->name, name) == 0) return b->teams[i];
    }
    return NULL;
}

void add_team(struct scoreboard * b, char * name) {
    if (b->num_teams >= b->max_num_teams || strlen(name) > 20) exit(EXIT_FAILURE);
    int i = b->num_teams;
    b->num_teams++;
    b->teams[i] = malloc(sizeof(struct team));
    b->teams[i]->name = malloc((strlen(name) + 1) * sizeof(char));
    strcpy(b->teams[i]->name, name);
    b->teams[i]->games = 0;
    b->teams[i]->points = 0;
}

struct team * get_team(struct scoreboard * b, char * name) {
    struct team * t = find_team(b, name);
    if (!t) add_team(b, name);
    t = find_team(b, name);
    return t;
}

void delete_board(struct scoreboard * b) {
    for (int i = 0; i < b->num_teams; i++) {
        free(b->teams[i]->name);
        free(b->teams[i]);
    }
    free(b->teams);
    free(b);
}

void add_match(struct scoreboard * b, char * name_t1, char * name_t2, int s1, int s2) {
    struct team * t1 = get_team(b, name_t1);
    struct team * t2 = get_team(b, name_t2);
    t1->games++;
    t2->games++;
    if (s1 < s2) {
        t2->points += 3;
    }
    if (s1 > s2) {
        t1->points += 3;
    }
    if (s1 == s2) {
        t1->points += 1;
        t2->points += 1;
    }
}


int should_be_first(struct team * t1, struct team * t2) {
    if (t1->points > t2->points) return 1;
    if (t1->points < t2->points) return 0;
    if (t1->points == t2->points) {
        if (strcmp(t1->name, t2->name) < 0) return 1;
        if (strcmp(t1->name, t2->name) > 0) return 0;
    }
    exit(EXIT_FAILURE);
}

void sort_board(struct scoreboard * b) {
    for (int i = 0; i < b->num_teams; i++) {
        for (int j = i + 1; j < b->num_teams; j++) {
            if (should_be_first(b->teams[j], b->teams[i])) {
                struct team * tempteam = b->teams[i];
                b->teams[i] = b->teams[j];
                b->teams[j] = tempteam;
            }
        }
    }
}

void print_board(struct scoreboard * b, FILE * output) {
    for (int i = 0; i < b->num_teams; i++) {
        fprintf(output, "%s %d %d\n", b->teams[i]->name, b->teams[i]->points, b->teams[i]->games);
    }
}


int parse_number(char * * pp) {
    if (!isdigit(**pp)) exit(EXIT_FAILURE);
    int n = 0;
    while (isdigit(**pp)) {
        n = 10 * n + (**pp - '0');
        (*pp)++;
    }
    return n;
}

void process_line(char * line, struct scoreboard * b) {
    char * p = line;
    while (isblank(*p)) p++;
    if (*p == '\0') return; // ignore blank and empty lines

    // team 1
    if (!isalnum(*p)) exit(EXIT_FAILURE);
    char * name_t1 = p;
    while (isalnum(*p)) p++;
    if (!isblank(*p)) exit(EXIT_FAILURE);
    *p = '\0';
    p++;

    // skip spaces
    while (isblank(*p)) p++;

    // team 2
    if (!isalnum(*p)) exit(EXIT_FAILURE);
    char * name_t2 = p;
    while (isalnum(*p)) p++;
    if (!isblank(*p)) exit(EXIT_FAILURE);
    *p = '\0';
    p++;

    // skip spaces
    while (isblank(*p)) p++;

    // score 1
    int s1 = parse_number(&p);
    if (!isblank(*p)) exit(EXIT_FAILURE);

    // skip spaces
    while (isblank(*p)) p++;

    // score 1
    int s2 = parse_number(&p);

    // make sure only blanks follow
    while (isblank(*p)) p++;
    if (*p != '\0') exit(EXIT_FAILURE);

    add_match(b, name_t1, name_t2, s1, s2);
}

int get_line(char * * linep, FILE * input) {
    unsigned i = 0;
    int c;
    while (1) {
        c = getc(input);
        if ((c == EOF && i > 0) || (c == '\n')) {
            (*linep)[i] = '\0';
            return 1;
        } else if (c == EOF && i == 0) {
            return 0;
        }
        (*linep)[i] = c;
        i++;
        *linep = realloc(*linep, (i + 1));
    }
}


int main(int argc, char * argv[]) {

    int max_teams;
    if (argc == 1) {
        max_teams = 10;
    } else {
        char * q = argv[1];
        max_teams = parse_number(&q);
    }
    
    struct scoreboard * b = new_board(max_teams);

    char * line = malloc(sizeof(char));

    while (get_line(&line, stdin)) {
        process_line(line, b);
    }

    free(line);

    sort_board(b);

    print_board(b, stdout);

    delete_board(b);

    exit(EXIT_SUCCESS);
}
