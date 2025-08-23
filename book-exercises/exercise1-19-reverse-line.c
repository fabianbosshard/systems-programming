#include <stdio.h>

#define MAXLINE 1000

int get_line(char line[], int max_line_len);
void reverse_line(char line[]);

void swap(char line[], int i, int j) {
    int temp = line[i];
    line[i] = line[j];
    line[j] = temp;
}

int main(void) {
  int len;
  char line[MAXLINE];

  while ((len = get_line(line, MAXLINE)) > 0) {
    reverse_line(line);
    printf("%s\n", line);
  }
}

int get_line(char line[], int max_line_len) {
  int c, i;

  for (i = 0; i < max_line_len - 1 && (c = getchar()) != EOF && c != '\n'; ++i) {
    line[i] = c;
  }

  line[i] = '\0';

  return i; // length
}

void reverse_line(char line[]) {
    int i, j;
    i = j = 0;
    for (j = 0; line[j] != '\0' && line[j] != '\n'; j++);
    if (line[j] == '\n') {
      line[j--] = '\0';
    }
    j--; // we do not want to swap the null character to the first position!
    while(i < j){
      swap(line, i++, j--);
    } 
}
