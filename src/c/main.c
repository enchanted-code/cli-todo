#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_TODO_FP "todos.txt"

void show_no_args_message() {
  fprintf(stderr, "Arguments need to be supplied. Use --help.");
  exit(EXIT_FAILURE);
}

void show_help_message() {
  puts("Todo cli written in C, by Leo Spratt");
  puts("Usage:");
  puts("\t(-i --interactive) open in interactive mode");
  puts("\t(-h --help) show this message");
  puts("Configure:");
  puts("\t TODO_FILENAME where the todo file will be");
  exit(EXIT_SUCCESS);
}

void write_todo(char *title, char *due_date, char *todo_fp) {
  FILE *fp;
  fp = fopen(todo_fp, "a");
  fprintf(fp, "title=\"%s\",due_date=%s\n", title, due_date);
  fclose(fp);
}

void read_todos(char *todo_fp) {
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  fp = fopen(todo_fp, "r");

  if (fp == NULL) {
    fprintf(stderr, "Todo file not found.");
    exit(EXIT_FAILURE);
  }

  while ((read = getline(&line, &len, fp)) != -1) {
    printf("%s", line);
  }
}

/**
  gets input from stdin, handling buffer overflows
  and removing the newline from end of input.

  @param str - the char array to store input
  @param n - the max length of input (including final null-character)
*/
void get_stdin_line(char *str, int n) {
  fgets(str, n, stdin);
  int last_i = strlen(str) - 1;
  int ch;
  // handle the line being to long
  if (str[last_i] != '\n') {
    while (((ch = getchar()) != '\n') && (ch != EOF)) {
    }
    str[last_i] = '\n';
  }
  // remove newline \n
  str[last_i] = 0;
}

void interactive_add(char *todo_fp) {
  // define variables
  char title[100];
  char due_date[12];
  // get inputs from user
  printf("Title: ");
  get_stdin_line(title, 100);
  printf("Due Date: ");
  get_stdin_line(due_date, 12);
  // write the todo to file
  write_todo(title, due_date, todo_fp);
}

void interactive_read(char *todo_fp) { read_todos(todo_fp); }

void interactive_mode(char *todo_fp) {
  char menu_choice[3];
  printf("(a)dd, (r)ead: ");
  get_stdin_line(menu_choice, 3);
  if (strcmp(menu_choice, "a") == 0) {
    interactive_add(todo_fp);
  } else if (strcmp(menu_choice, "r") == 0) {
    interactive_read(todo_fp);
  } else {
    printf("%s %s", "Unknown choice:", menu_choice);
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char *argv[]) {
  char *todo_fp = getenv("TODO_FILENAME");
  if (!todo_fp) {
    todo_fp = DEFAULT_TODO_FP;
  }

  if (argc == 1) {
    show_no_args_message();
  } else if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
    show_help_message();
  } else if (strcmp(argv[1], "-i") == 0 ||
             strcmp(argv[1], "--interactive") == 0) {
    interactive_mode(todo_fp);
  }
}
