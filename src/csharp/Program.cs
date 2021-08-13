using System;
using System.IO;
using System.Globalization;

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
            switch (args[0])
            {
                case "-h":
                case "--help":
                    show_help_message();
                    break;
                case "-i":
                case "--interactive":
                    interactive_mode(todo_fn);
                    break;
                default:
                    show_invalid_args_message();
                    break;
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
            due_date_str = due_date.ToString("%yyyy-%MM-%dd");
            File.AppendAllText(
                todo_fp,
                String.Format("title={0},due_date={1}\n", title, due_date_str)
            );
        }
        static private void write_todo(string title, string todo_fp)
        {
            if (!File.Exists(todo_fp))
            {
                Console.Error.WriteLine("Todo file not found");
                Environment.Exit(1);
            }
            File.AppendAllText(
                todo_fp,
                String.Format("title={0},due_date=\n", title)
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
                    File.AppendAllText(tempPath, line + "\n");
                }
                currLine++;
            }
            File.Delete(todo_fp);
            File.Move(tempPath, todo_fp);
        }
        static private void interactive_add(string todo_fp)
        {
            Console.Write("Title: ");
            string title = Console.ReadLine();
            Console.Write("Due Date: ");
            string due_date_str = Console.ReadLine();
            DateTime dueDateTime;
            if (!String.IsNullOrEmpty(due_date_str))
            {
                CultureInfo provider = CultureInfo.InvariantCulture;
                dueDateTime = DateTime.ParseExact(due_date_str, "%yyyy-%MM-%dd", provider);
                write_todo(title, dueDateTime, todo_fp);
            }
            else
            {
                write_todo(title, todo_fp);
            }
        }
        static private void interactive_read(string todo_fp)
        {
            Console.Write("(a)ll, (o)ne, (c)ount: ");
            string choice = Console.ReadLine().ToLower();
            if (choice.StartsWith("a"))
            {
                read_todos(todo_fp);
            }
            else if (choice.StartsWith("o"))
            {
                Console.Write("Number: ");
                var selected_line = int.Parse(Console.ReadLine());
                read_todo(selected_line, todo_fp);
            }
            else if (choice.StartsWith("c"))
            {
                Console.WriteLine(count_todos(todo_fp));
            }
            else
            {
                Console.Error.WriteLine("Unknown choice: {0}", choice);
                Environment.Exit(1);
            }
        }
        static private void interactive_delete(string todo_fp)
        {
            Console.Write("(a)ll, (o)ne: ");
            string choice = Console.ReadLine().ToLower();
            if (choice.StartsWith("a"))
            {
                delete_todos(todo_fp, true);
            }
            else if (choice.StartsWith("o"))
            {
                Console.Write("Number: ");
                var selected_line = int.Parse(Console.ReadLine());
                delete_todo(selected_line, todo_fp);
            }
            else
            {
                Console.Error.WriteLine("Unknown choice: {0}", choice);
                Environment.Exit(1);
            }
        }
        static private void interactive_mode(string todo_fp)
        {
            Console.Write("(a)dd, (r)ead, (d)elete: ");
            string choice = Console.ReadLine().ToLower();
            if (choice.StartsWith("a"))
            {
                interactive_add(todo_fp);
            }
            else if (choice.StartsWith("r"))
            {
                interactive_read(todo_fp);
            }
            else if (choice.StartsWith("d"))
            {
                interactive_delete(todo_fp);
            }
            else
            {
                Console.Error.WriteLine("Unknown choice: {0}", choice);
                Environment.Exit(1);
            }
        }
        static private void command_add_() { }
        static private void command_view() { }
        static private void command_delete() { }
    }
}
