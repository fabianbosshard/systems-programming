#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXLEN 1000


struct letter {
    char letter;
    int w;
    int h;
    int d;
    int h_rel; // h_rel = h - d  (i.e. above baseline)
    int h_abs; // h_abs = h_rel + depth_max
    int b; // begin index
    int e; // end index (one after)
    char * appearance;
};

int altitude_max, depth_max;
int height_total; // = altitude_max + depth_max

struct letter format[256];


int get_num(FILE * fp, int * cp) { 
    int num = 0;
    while (isdigit((unsigned char)(*cp = getc(fp)))) {
        num = 10 * num + (*cp - '0');
    }
    return num;
}

int get_formatting(FILE * fp) {
    for (int i = 0; i < 256; i++) {
        format[i].letter = i;
        format[i].w = 1;
        format[i].h = 1;
        format[i].d = 0;
        format[i].appearance = malloc(sizeof(char)); // same as realloc(NULL, sizeof(char))
        format[i].appearance[0] = i;
    }
    int c;
    while ((c = getc(fp)) != EOF) {
        if (c == '\n' || c == '\r') continue; // ignore empty lines
        unsigned char cur = (unsigned char)c; c = getc(fp); if (c != ' ') return 1;
        int width = get_num(fp, &c); if (c != ' ') return 1;
        int height = get_num(fp, &c); if (c != ' ') return 1;
        int depth = get_num(fp, &c); if (c != ':') return 1;
        char * appear = realloc(format[cur].appearance, sizeof(char) * width * height);
        for (int i = 0; i < width * height; i++) {
            appear[i] = getc(fp);
        }
        if (width <= 0 || height <= 0 || depth < 0 || depth > height) return 1;
        format[cur].letter = cur;
        format[cur].w = width;
        format[cur].h = height;
        format[cur].d = depth;
        format[cur].appearance = appear;
        c = getc(fp); if (c != '\n' && c != EOF) return 1;
    }
    altitude_max = 1; // altitude = height - depth  (i.e. the height above the baseline)
    depth_max = 0;
    for (int i = 0; i < 256; i++) {
        if (format[i].h - format[i].d > altitude_max) altitude_max = format[i].h - format[i].d;
        if (format[i].d > depth_max) depth_max = format[i].d;
    }
    height_total = altitude_max + depth_max;
    for (int i = 0; i < 256; i++) {
        format[i].h_rel = format[i].h - format[i].d;
        format[i].h_abs = format[i].h_rel + depth_max;
        format[i].b = height_total - format[i].h_abs;
        format[i].e = format[i].b + format[i].h;
    }
    return 0;
}

void free_formatting() {
    for (int i = 0; i < 256; i++) {
        free(format[i].appearance);
    }
}


void render_line(char * line) {
    static int first = 1;
    int * linelengths = malloc(height_total * sizeof(int));
    for (int i = 0; i < height_total; i++) linelengths[i] = 0; // initialize all linelengths to 0

    int cur_width = 0;
    for (int k = 0; k < strlen(line); k++) {
        if (k > 0) cur_width++; // horizontal separation of 1 space
        cur_width += format[line[k]].w;
        for (int i = format[line[k]].b; i < format[line[k]].e; i++) {
            linelengths[i] = cur_width;
        }
    }

    char ** lines = malloc(height_total * sizeof(char *));
    for (int i = 0; i < height_total; i++) {
        lines[i] = malloc((linelengths[i] + 1) * sizeof(char));
        for (int j = 0; j < linelengths[i]; j++) {
            lines[i][j] = ' ';
        }
        lines[i][linelengths[i]] = '\0';
    }

    int offset = 0;
    for (int k = 0; k < strlen(line); k++) {
        if (k > 0) offset++; // horizontal separation of 1 space
        for (int i = format[line[k]].b; i < format[line[k]].e; i++) {
            for (int j = offset; j < offset + format[line[k]].w; j++) {
                lines[i][j] = format[line[k]].appearance[(i - format[line[k]].b) * format[line[k]].w + (j - offset)];
            }
        }
        offset += format[line[k]].w;
    }

    first ? first = 0 : putchar('\n');
    for (int i = 0; i < height_total; i++) {
        printf("%s\n", lines[i]);
    }

    for (int i = 0; i < height_total; i++) free(lines[i]);
    free(lines);
    free(linelengths);
}

void process_lines() {
    char line[MAXLEN + 1]; // space for '\0'
    while (fgets(line, MAXLEN + 1, stdin)) {
        if (strlen(line) > 0 && line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }
        render_line(line);
    }
}

void show() {
    for (int k = 0; k < 256; k++) {
        for (int i = 0; i < format[k].h; i++) {
            for (int j = 0; j < format[k].w; j++) {
                putchar(format[k].appearance[i * format[k].w + j]);
            }
            putchar('\n');
        }
        printf("\n");
    }
}



int main(int argc, char * argv[]) {
    FILE * fp = NULL;
    if (argc == 1) {
        if ((fp = fopen("FONT", "r"))) {
            if (get_formatting(fp)) return 1;
            fclose(fp);
        }
    } else if (argc == 2) {
        if ((fp = fopen(argv[1], "r"))) {
            if (get_formatting(fp)) return 1;
            fclose(fp);
        }
    } else {
        return 1;
    }
    process_lines();
    free_formatting();
    return 0;
}
