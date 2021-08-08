#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TODO_FP "todos.txt"

void show_no_args_message() {
  fprintf(stderr, "Arguments need to be supplied. Use --help.");
  exit(EXIT_FAILURE);
}

void show_help_message() {
  puts("Todo cli written in C, by Leo Spratt");
  puts("Usage:");
  puts("\t(-i --interactive) open in interactive mode");
  puts("\t(-h --help) show this message");
  exit(EXIT_SUCCESS);
}

void write_todo(char *title, char *due_date) {
  FILE *fp;
  fp = fopen(TODO_FP, "a");
  fprintf(fp, "title=\"%s\",due_date=%s\n", title, due_date);
  fclose(fp);
}

void read_todos() {
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  fp = fopen(TODO_FP, "r");

  if (fp == NULL) {
    exit(EXIT_FAILURE);
  }

  while ((read = getline(&line, &len, fp)) != -1) {
    printf("%s", line);
  }
}

void interactive_add() {
  // define variables
  char title[100];
  char due_date[12];
  // get inputs from user
  printf("Title: ");
  fgets(title, 100, stdin);
  printf("Due Date: ");
  fgets(due_date, 12, stdin);
  // remove \n from strings
  title[strlen(title) - 1] = 0;
  due_date[strlen(due_date) - 1] = 0;
  // write the todo to file
  write_todo(title, due_date);
}

void interactive_read() { read_todos(); }

void interactive_mode() {
  char menu_choice[3];
  printf("(a)dd, (r)ead: ");
  fgets(menu_choice, 3, stdin);
  menu_choice[strlen(menu_choice) - 1] = 0;

  if (strcmp(menu_choice, "a") == 0) {
    interactive_add();
  } else if (strcmp(menu_choice, "r") == 0) {
    interactive_read();
  } else {
    printf("%s %s", "Unknown choice:", menu_choice);
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char *argv[]) {
  if (argc == 1) {
    show_no_args_message();
  } else if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
    show_help_message();
  } else if (strcmp(argv[1], "-i") == 0 ||
             strcmp(argv[1], "--interactive") == 0) {
    interactive_mode();
  }
}
