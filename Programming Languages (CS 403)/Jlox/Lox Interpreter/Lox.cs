using System.Text;

namespace Lox_Interpreter
{
    /// <summary>
    /// Represents the entry point to the Jlox interepreter. 
    /// It is responsible for reading and running Lox files/lines of code and reporting errors.
    /// </summary>
    public class Lox
    {
        //this variable is used to indicate when an error has been encountered
        public static bool hadError = false;

        /// <summary>
        /// Handles when and how to run jlox, either in line-by-line in the terminal or runnning a file specified by path.
        /// </summary>
        /// <param name="args">The command line arguments on which to run jlox. Optional specification of file name or path is allowed.</param>
        public static void Main(String[] args)
        {
            if (args.Length > 1)
            {
                Console.WriteLine("Usage: jlox [script]");
                System.Environment.Exit(64);
            }
            else if (args.Length == 1) // represents the case "jlox [filePath]"
            {
                RunFile(args[0]);
            }
            else // represents the case "jlox"
            {
                RunPrompt();
            }
        }

        /// <summary>
        /// Takes a file name or path and executes the contents of that file. 
        /// If an error occurs in running the code, the code will stop being executed.
        /// </summary>
        /// <param name="path">File name or path of file that is being interpreted.</param>
        private static void RunFile(String path)
        {
            Encoding utf8 = new UTF8Encoding(true);
            byte[] bytes = File.ReadAllBytes(Path.GetFullPath(path));
            Run(new string(utf8.GetString(bytes)));
            if (hadError) System.Environment.Exit(65);
        }

        /// <summary>
        /// Allows the user to enter jlox code line by line in a terminal and have their code be executed line by line until EOF.
        /// To end a file, type CTRL+Z+ENTER.
        /// </summary>
        private static void RunPrompt()
        {
            using StreamReader input = new(Console.OpenStandardInput());
            string? line;
            Console.Write("> ");
            while ((line = input.ReadLine()) != null)
            {
                Run(line);
                hadError = false;
                Console.Write("> ");
            }
        }

        /// <summary>
        /// Runs the source code by using a scanner to scan in tokens and then execute it.
        /// </summary>
        /// <param name="source">The file contents or line of code to be executed.</param>
        private static void Run(String source)
        {
            Scanner scanner = new(source);
            List<Token> tokens = scanner.ScanTokens();

            // For now, just print the tokens.
            foreach (Token token in tokens)
            {
                Console.WriteLine(token);
            }

        }

        /// <summary>
        /// Calls <see cref="Report(int, string, string)"/> to report an error.
        /// </summary>
        /// <param name="line">Line where the error was found.</param>
        /// <param name="message">Error message to be printed.</param>
        public static void Error(int line, String message)
        {
            Report(line, "", message);
        }

        /// <summary>
        /// Prints an error message to the terminal and changes the Lox class to be in an error state.
        /// </summary>
        /// <param name="line">Line where the error was found.</param>
        /// <param name="where">Location in line where error occurs.</param>
        /// <param name="message">Error message to be printed.</param>
        private static void Report(int line, String where, String message)
        {
            Console.Error.WriteLine("[line " + line + "] Error" + where + ": " + message);
            hadError = true;
        }

    }
}

