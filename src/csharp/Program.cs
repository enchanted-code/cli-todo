using System;

namespace csharp
{
    class Program
    {
        public const string DEFAULT_TODO_FP = "todos.txt";
        public const string TEMP_FILE_EXT = ".tmp";
        static void Main(string[] args)
        {
            var todo_fn = Environment.GetEnvironmentVariable("TODO_FILENAME");
            if (String.IsNullOrEmpty(todo_fn))
            {
                todo_fn = DEFAULT_TODO_FP;
            }
            if (args.Length == 0)
            {
                show_no_args_message();
            }
            if (args[0] == "-h" || args[0] == "--help")
            {
                show_help_message();
            }
        }
        static private void show_no_args_message()
        {
            Console.Error.WriteLine("Arguments need to be supplied. Use --help");
            Environment.Exit(1);
        }
        static private void show_invalid_args_message()
        {
            Console.Error.WriteLine("Invalid arguments supplied. Use --help");
            Environment.Exit(1);
        }
        static private void show_missing_args_message()
        {
            Console.Error.WriteLine("Missing required arguments. Use --help");
            Environment.Exit(1);
        }
        static private void show_help_message()
        {
            Console.WriteLine("Todo cli written in C, by Leo Spratt");
            Console.WriteLine("Usage:");
            Console.WriteLine("\t(-i --interactive) open in interactive mode");
            Console.WriteLine("\t(-a --add) add a todo");
            Console.WriteLine("\t\t--title=\"<title>\" the title");
            Console.WriteLine("\t\t--due=\"<yyyy-mm-dd>\" the due date");
            Console.WriteLine("\t\t(-s --silent) don't show output on success");
            Console.WriteLine("\t(-v --view) view todos");
            Console.WriteLine("\t\t--all show all todos");
            Console.WriteLine("\t\t--one show one todo, use --line to choose which");
            Console.WriteLine("\t\t--line=<int> select a specific line");
            Console.WriteLine("\t\t--count show total count of todos");
            Console.WriteLine("\t(-d --delete) remove a todo");
            Console.WriteLine("\t\t--all remove all");
            Console.WriteLine("\t\t--line=<int> remove a specific line");
            Console.WriteLine("\t(-h --help) show this message");
            Console.WriteLine("Configure:");
            Console.WriteLine("\t TODO_FILENAME where the todo file will be");
        }
        static private void write_todo() { }
        static private void read_todo() { }
        static private void read_todos() { }
        static private int count_todos() { return 0; }
        static private void delete_todos() { }
        static private void delete_todo() { }
        static private void interactive_add() { }
        static private void interactive_read() { }
        static private void interactive_delete() { }
        static private void interactive_mode() { }
        static private void command_add_() { }
        static private void command_view() { }
        static private void command_delete() { }
    }
}
