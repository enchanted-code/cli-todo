# Cli Todo
A command line interface for creating and showing a todo list.

This project is meant to illustrate writing a todo program in different programming languages. Each language is placed in a different directory for easy navigation. Makefiles are used and are placed in each language directory that requires compiling before use. Built executables will go into the /build folder.

Each version should have the same interface, *Depending on each language's feature set*. They also should not need any other external libraries.

## Program Usage
### Command Flags/Arguments
- `(-i --interactive)` open in interactive mode
- `(-a --add)` add a todo
    - `--title=<str>`the title
    - `--due=<yyyy-mm-dd>` add a due date
    - `-s --silent` don't show output on success
- `(-v --view)` view todos
    - `--all` show all todos
    - `--one` show one todo
    - `--line=<int>` select a specific line
    - `--count` count total todos
- `(-d --delete)` remove a todo
    - `--all` remove all
    - `--line=<int>` remove a specific line
- `(-h --help)` show this message

### Environment Variables
- `TODO_FILENAME` where the todo file is located

## Current Languages
- C

## License
Copyright (C) 2021  Leo Spratt

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
