using System;
using System.IO;

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
        static private void write_todo(string title, DateTime due_date, string todo_fp)
        {
            if (!File.Exists(todo_fp))
            {
                Console.Error.WriteLine("Todo file not found");
                Environment.Exit(1);
            }
            string due_date_str = "";
            if (due_date != null)
            {
                due_date_str = due_date.ToString("%yyyy-%MM-%dd");
            }
            File.AppendAllText(
                todo_fp,
                String.Format("title={0},due_date={1}\n", title, due_date_str)
            );
        }
        static private void read_todos(string todo_fp)
        {
            if (!File.Exists(todo_fp))
            {
                Console.Error.WriteLine("Todo file not found");
                Environment.Exit(1);
            }
            foreach (string line in File.ReadLines(todo_fp))
            {
                Console.WriteLine(line);
            }
        }
        static private void read_todo(int selected_line, string todo_fp)
        {
            if (!File.Exists(todo_fp))
            {
                Console.Error.WriteLine("Todo file not found");
                Environment.Exit(1);
            }
            int currLine = 1;
            foreach (string line in File.ReadLines(todo_fp))
            {
                if (currLine == selected_line)
                {
                    Console.WriteLine(line);
                    break;
                }
                currLine++;
            }
        }
        static private int count_todos(string todo_fp)
        {
            if (!File.Exists(todo_fp))
            {
                Console.Error.WriteLine("Todo file not found");
                Environment.Exit(1);
            }
            int currLine = 0;
            foreach (string line in File.ReadLines(todo_fp))
            {
                currLine++;
            }
            return currLine;
        }
        static private void delete_todos(string todo_fp, bool create_new_file)
        {
            File.Delete(todo_fp);
            if (create_new_file)
            {
                File.Create(todo_fp);
            }
        }
        static private void delete_todo(int selected_line, string todo_fp)
        {
            if (!File.Exists(todo_fp))
            {
                Console.Error.WriteLine("Todo file not found");
                Environment.Exit(1);
            }
            string tempPath = todo_fp + TEMP_FILE_EXT;
            int currLine = 1;
            foreach (string line in File.ReadLines(todo_fp))
            {
                if (currLine != selected_line)
                {
                    File.AppendAllText(tempPath, line);
                }
                currLine++;
            }
            File.Delete(todo_fp);
            File.Move(tempPath, todo_fp);
        }
        static private void interactive_add() { }
        static private void interactive_read() { }
        static private void interactive_delete() { }
        static private void interactive_mode() { }
        static private void command_add_() { }
        static private void command_view() { }
        static private void command_delete() { }
    }
}
