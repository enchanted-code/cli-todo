#!/usr/bin/env python3
import linecache
import os
import sys
from datetime import datetime
from pathlib import Path

DEFAULT_TODO_FP = "todos.txt"
TEMP_FILE_EXT = ".tmp"

def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

def show_no_args_message():
    eprint("Arguments need to be supplied. Use --help")
    sys.exit(1)

def show_invalid_args_message():
    eprint("Invalid arguments supplied. Use --help")
    sys.exit(1)

def show_missing_args_message():
    eprint("Missing required arguments. Use --help")
    sys.exit(1)

def show_help_message():
  print("Todo cli written in C, by Leo Spratt")
  print("Usage:")
  print("\t(-i --interactive) open in interactive mode")
  print("\t(-a --add) add a todo")
  print("\t\t--title=\"<title>\" the title")
  print("\t\t--due=\"<yyyy-mm-dd>\" the due date")
  print("\t\t(-s --silent) don't show output on success")
  print("\t(-v --view) view todos")
  print("\t\t--all show all todos")
  print("\t\t--one show one todo, use --line to choose which")
  print("\t\t--line=<int> select a specific line")
  print("\t\t--count show total count of todos")
  print("\t(-d --delete) remove a todo")
  print("\t\t--all remove all")
  print("\t\t--line=<int> remove a specific line")
  print("\t(-h --help) show this message")
  print("Configure:")
  print("\t TODO_FILENAME where the todo file will be")
  sys.exit(0)

def write_todo(title: str, due_date: datetime, todo_fp: Path):
    if not todo_fp.exists():
        eprint("Todo file not found")
        sys.exit(1)

    if isinstance(due_date, datetime):
        due_date = due_date.strftime("%Y-%m-%d")

    with todo_fp.open("a") as fo:
        fo.write(f"title=\"{title}\",due_date={due_date}\n")

def read_todos(todo_fp: Path):
    if not todo_fp.exists():
        eprint("Todo file not found")
        sys.exit(1)

    with todo_fp.open("r") as fo:
        lines = fo.readlines()
        for line in lines:
            print(line.strip("\n"))

def read_todo(selected_line: int, todo_fp: Path):
    if not todo_fp.exists():
       eprint("Todo file not found")
       sys.exit(1)

    with todo_fp.open("r") as fo:
        lines = fo.readlines()
        if len(lines) >= selected_line:
            print(lines[selected_line - 1].strip("\n"))

def count_todos(todo_fp: Path):
    if not todo_fp.exists():
       eprint("Todo file not found")
       sys.exit(1)

    with todo_fp.open("r") as fo:
        lines = fo.readlines()
        print(len(lines))

def delete_todos(todo_fp: Path, create_new_file: bool):
    todo_fp.unlink()
    if create_new_file:
        todo_fp.touch()

def delete_todo(selected_line: int, todo_fp: Path):
    if not todo_fp.exists():
        eprint("Todo file not found")
        sys.exit(1)

    temp_fn = Path(str(todo_fp) + TEMP_FILE_EXT)

    with todo_fp.open("r") as src_fo:
        with temp_fn.open("a") as dest_fo:
            current_line = 1
            for line in src_fo.readlines():
                if current_line != selected_line:
                    dest_fo.write(line)
                current_line += 1

    todo_fp.unlink()
    temp_fn.rename(todo_fp)

def interactive_add(todo_fn: Path):
    title = input("Title: ")
    due_date = input("Due Date: ")
    if due_date:
        due_date = datetime.strptime(due_date, "%Y-%m-%d")
    write_todo(title, due_date, todo_fn)

def interactive_read(todo_fn: Path):
    choice = input("(a)ll, (o)ne, (c)ount: ").lower()
    if choice.startswith("a"):
        read_todos(todo_fn)
    elif choice.startswith("o"):
        selected_line = int(input("Number: "))
        read_todo(selected_line, todo_fn)
    elif choice.startswith("c"):
        count_todos(todo_fn)
    else:
        eprint("Unknown choice: ", choice)
        sys.exit(1)

def interactive_delete(todo_fn: Path):
    choice = input("(a)ll, (o)ne: ").lower()
    if choice.startswith("a"):
        delete_todos(todo_fn, True)
    elif choice.startswith("o"):
        selected_line = int(input("Number: "))
        delete_todo(selected_line, todo_fn)
    else:
        eprint("Unknown choice: ", choice)
        sys.exit(1)

def interactive_mode(todo_fn: Path):
    choice = input("(a)dd, (r)ead, (d)elete: ").lower()
    if choice.startswith("a"):
        interactive_add(todo_fn)
    elif choice.startswith("r"):
        interactive_read(todo_fn)
    elif choice.startswith("d"):
        interactive_delete(todo_fn)
    else:
        eprint("Unknown choice: ", choice)
        sys.exit(1)

def command_add(argv: tuple, todo_fp: Path):
    title = None
    due = ""
    silent = False

    for arg in argv:
        if arg.startswith("--title="):
            title = arg[8:]
        elif arg.startswith("--due="):
            due = arg[6:]
            due = datetime.strptime(due, "%Y-%m-%d")
        elif arg == "-s" or arg == "--silent":
            silent = True

    if not title:
        show_missing_args_message()

    write_todo(title, due, todo_fp)

    if not silent:
        print("Title:", title)
        if isinstance(due, datetime):
            due = due.strftime("%Y-%d-%m")
        print("Date Due:", due)

def command_view(argv: tuple, todo_fp: Path):
    count = False
    show_all = False
    one = False
    selected_line = 1

    for arg in argv:
        if arg == "--count":
            count = True
        elif arg == "--all":
            show_all = True
        elif arg == "--one":
            one = True
        elif arg.startswith("--line="):
            selected_line = int(arg[7:])

    if count:
        count_todos(todo_fp)
    if show_all:
        read_todos(todo_fp)
    elif one:
        read_todo(selected_line, todo_fp)

def command_delete(argv: tuple, todo_fp: Path):
    select_all = False
    one = False
    selected_line = 1

    for arg in argv:
        if arg == "--all":
            show_all = True
        elif arg == "--one":
            one = True
        elif arg.startswith("--line="):
            selected_line = int(arg[7:])

    if select_all:
        delete_todos(todo_fp)
    elif one:
        delete_todo(selected_line, todo_fp)

def main(argc: int, argv: tuple):
    todo_fn = Path(os.environ.get("TODO_FILENAME", DEFAULT_TODO_FP))

    if argc == 1:
        show_no_args_message()
    elif argv[1] in ("-h", "--help"):
        show_help_message()
    elif argv[1] in ("-i", "--interactive"):
        interactive_mode(todo_fn)
    elif argv[1] in ("-a", "--add"):
        command_add(argv, todo_fn)
    elif argv[1] in ("-v", "--view"):
        command_view(argv, todo_fn)
    elif argv[1] in ("-d", "--delete"):
        command_delete(argv, todo_fn)
    else:
        show_invalid_args_message()

if __name__ == "__main__":
    try:
        main(len(sys.argv), sys.argv)
    except KeyboardInterrupt:
        sys.exit(0)
