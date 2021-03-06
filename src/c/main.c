#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_TODO_FP "todos.txt"
#define TEMP_FILE_EXT ".tmp"

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
  puts("\t(-v --view) view todos");
  puts("\t\t --all show all todos");
  puts("\t\t --one show one todo, use --line to choose which");
  puts("\t\t --line=<int> select a specific line");
  puts("\t\t --count show total count of todos");
  puts("\t(-d --delete) remove a todo");
  puts("\t\t--all remove all");
  puts("\t\t--line=<int> remove a specific line");
  puts("\t(-h --help) show this message");
  puts("Configure:");
  puts("\t TODO_FILENAME where the todo file will be");
  exit(EXIT_SUCCESS);
}

void append_char(char *dest, char *src, char *to_append) {
  strcpy(dest, src);
  int src_len = strlen(src);
  int total_len = src_len + strlen(to_append);
  int append_i = 0;
  for (int i = src_len; i < total_len + 1; i++) {
    dest[i] = to_append[append_i];
    append_i++;
  }
}

void write_todo(char *title, char *due_date, char *todo_fp) {
  FILE *fp;
  fp = fopen(todo_fp, "a");
  fprintf(fp, "title=\"%s\",due_date=%s\n", title, due_date);
  fclose(fp);
}

void delete_todos(char *todo_fp, bool recreate_file) {
  remove(todo_fp);
  if (recreate_file) {
    FILE *fp = fopen(todo_fp, "w");
    fclose(fp);
  }
}

void delete_todo(int selected_line, char *todo_fp) {
  FILE *source_f;
  FILE *temp_f;
  int line_count = 0;
  char ch;
  char temp_filename[strlen(todo_fp) + strlen(TEMP_FILE_EXT)];
  // extend todo filename with .tmp extention
  append_char(temp_filename, todo_fp, TEMP_FILE_EXT);
  source_f = fopen(todo_fp, "r");
  // make sure todo source exists
  if (source_f == NULL) {
    fprintf(stderr, "Todo file not found.");
    exit(EXIT_FAILURE);
  }
  temp_f = fopen(temp_filename, "a");

  // read from soure and write into temp
  while ((ch = fgetc(source_f)) != EOF) {
    if (line_count != selected_line) {
      // we want anything other than the selected line
      fprintf(temp_f, "%c", ch);
    }
    if (ch == '\n') {
      line_count++;
    }
  }
  fclose(source_f);
  fclose(temp_f);
  // replace source with the temp
  remove(todo_fp);
  rename(temp_filename, todo_fp);
}

int count_todos(char *todo_fp) {
  FILE *fp;
  char ch;
  int line_count = 0;
  fp = fopen(todo_fp, "r");

  if (fp == NULL) {
    fprintf(stderr, "Todo file not found.");
    exit(EXIT_FAILURE);
  }

  while ((ch = fgetc(fp)) != EOF) {
    if (ch == '\n') {
      line_count++;
    }
  }
  fclose(fp);
  return line_count;
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
  fclose(fp);
}

void read_todo(char *todo_fp, int n) {
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  int curr_line = 0;
  fp = fopen(todo_fp, "r");

  if (fp == NULL) {
    fprintf(stderr, "Todo file not found.");
    exit(EXIT_FAILURE);
  }

  while ((read = getline(&line, &len, fp)) != -1) {
    if (curr_line == n) {
      printf("%s", line);
      break;
    }
    curr_line++;
  }
  fclose(fp);
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

void interactive_read(char *todo_fp) {
  char menu_choice[3];
  printf("(a)ll (o)ne (c)ount: ");
  get_stdin_line(menu_choice, 3);
  if (strcmp(menu_choice, "a") == 0) {
    read_todos(todo_fp);
  } else if (strcmp(menu_choice, "o") == 0) {
    int selected_line = 1;
    printf("Number: ");
    scanf("%d", &selected_line);
    // makes sure we are using 0
    // indexed as users will start from one
    selected_line--;
    read_todo(todo_fp, selected_line);
  } else if (strcmp(menu_choice, "c") == 0) {
    int count = count_todos(todo_fp);
    printf("%d\n", count);
  }
}

void interactive_delete(char *todo_fp) {
  char menu_choice[3];
  int selected_line = 0;
  printf("(a)ll (o)ne: ");
  get_stdin_line(menu_choice, 3);
  if (strcmp(menu_choice, "a") == 0) {
    delete_todos(todo_fp, true);
  } else if (strcmp(menu_choice, "o") == 0) {
    printf("Number: ");
    scanf("%d", &selected_line);
    if (selected_line != 0) {
      // makes sure we are using 0
      // indexed as users will start from one
      selected_line--;
    }
    delete_todo(selected_line, todo_fp);

  } else {
    printf("%s %s", "Unknown choice:", menu_choice);
    exit(EXIT_FAILURE);
  }
}

void interactive_mode(char *todo_fp) {
  char menu_choice[3];
  printf("(a)dd, (r)ead, (d)elete: ");
  get_stdin_line(menu_choice, 3);
  if (strcmp(menu_choice, "a") == 0) {
    interactive_add(todo_fp);
  } else if (strcmp(menu_choice, "r") == 0) {
    interactive_read(todo_fp);
  } else if (strcmp(menu_choice, "d") == 0) {
    interactive_delete(todo_fp);
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

void command_view(int argc, char *argv[], char *todo_fp) {
  bool count = false;
  bool all = false;
  bool one = false;
  int selected_line = 0;
  char *buffer;

  for (int i = 1; i < argc; i++) {
    if (strncmp(argv[i], "--count", 7) == 0) {
      count = true;
    } else if (strncmp(argv[i], "--all", 5) == 0) {
      all = true;
    } else if (strncmp(argv[i], "--line=", 7) == 0) {
      buffer = malloc(strlen(argv[i]));
      strncpy(buffer, argv[i] + 7, strlen(argv[i]));
      selected_line = atoi(buffer);
      free(buffer);
    } else if (strncmp(argv[i], "--one", 5) == 0) {
      one = true;
    }
  }

  if (count) {
    int todo_count = count_todos(todo_fp);
    printf("%d\n", todo_count);
  }
  if (all) {
    read_todos(todo_fp);
  } else if (one) {
    if (selected_line != 0) {
      // user want 1 indexed but we want 0 indexed;
      selected_line--;
    }
    read_todo(todo_fp, selected_line);
  }
}

void command_delete(int argc, char *argv[], char *todo_fp) {
  bool all = false;
  bool one = false;
  int selected_line = 0;
  char *buffer;

  for (int i = 1; i < argc; i++) {
    if (strncmp(argv[i], "--all", 5) == 0) {
      all = true;
    } else if (strncmp(argv[i], "--line=", 7) == 0) {
      buffer = malloc(strlen(argv[i]));
      strncpy(buffer, argv[i] + 7, strlen(argv[i]));
      selected_line = atoi(buffer);
      free(buffer);
      one = true;
    }
  }
  if (all) {
    delete_todos(todo_fp, true);
  } else if (one) {
    if (selected_line != 0) {
      // user want 1 indexed but we want 0 indexed;
      selected_line--;
    }
    delete_todo(selected_line, todo_fp);
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
  } else if (strcmp(argv[1], "-a") == 0 || strcmp(argv[1], "--add") == 0) {
    command_add(argc, argv, todo_fp);
  } else if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--view") == 0) {
    command_view(argc, argv, todo_fp);
  } else if (strcmp(argv[1], "-d") == 0 || strcmp(argv[1], "--delete") == 0) {
    command_delete(argc, argv, todo_fp);
  } else {
    show_invalid_args_message();
  }
  return 0;
}
