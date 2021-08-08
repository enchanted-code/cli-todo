#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_TODO_FP "todos.txt"

void show_no_args_message() {
  fprintf(stderr, "Arguments need to be supplied. Use --help.");
  exit(EXIT_FAILURE);
}

void show_invalid_args_message() {
  fprintf(stderr, "Invalid arguments supplied. Use --help.");
  exit(EXIT_FAILURE);
}

void show_missing_args_message() {
  fprintf(stderr, "Missing required arguments. Use --help.");
  exit(EXIT_FAILURE);
}

void show_help_message() {
  puts("Todo cli written in C, by Leo Spratt");
  puts("Usage:");
  puts("\t(-i --interactive) open in interactive mode");
  puts("\t(-a --add) add a todo");
  puts("\t\t--title=\"<title>\" the title");
  puts("\t\t--due=\"<yyyy-mm-dd>\" the due date");
  puts("\t\t(-s --silent) don't show output on success");
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

void command_add(int argc, char *argv[], char *todo_fp) {
  // define variables
  char *title;
  char *due;
  bool silent = false;
  // process arguments
  for (int i = 1; i < argc; i++) {
    if (strncmp(argv[i], "--title=", 8) == 0) {
      // reserve space for title
      title = malloc(strlen(argv[i]));
      // substring to get just value
      strncpy(title, argv[i] + 8, strlen(argv[i]) - 1);
    } else if (strncmp(argv[i], "--due=", 6) == 0) {
      // reserve space for due
      due = malloc(strlen(argv[i]));
      // substring to get just value
      strncpy(due, argv[i] + 6, strlen(argv[i]) - 1);
    } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--silent") == 0) {
      silent = true;
    }
  }
  // make sure values are correct
  if (title == NULL) {
    show_missing_args_message();
  }
  if (due == NULL) {
    due = "";
  }
  // output to give same format as "interactive mode"
  if (!silent) {
    printf("Title: %s\n", title);
    printf("Date Due: %s\n", due);
  }
  // write to file
  write_todo(title, due, todo_fp);
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
  } else if (strcmp(argv[1], "-a") == 0 || strcmp(argv[1], "--add") == 0) {
    command_add(argc, argv, todo_fp);
  } else if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--view") == 0) {
    interactive_read(todo_fp);
  } else {
    show_invalid_args_message();
  }
  return 0;
}
