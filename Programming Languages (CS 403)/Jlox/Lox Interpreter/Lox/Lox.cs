using System;
using System.IO;
using System.Collections.Generic;
using System.Text;

namespace Lox_Interpreter.Lox
{
    /// <summary>
    /// Represents the entry point to the Jlox interepreter. 
    /// It is responsible for reading and running Lox files/lines of code and reporting errors.
    /// </summary>
    internal class Lox
    {
        // The actual interpreter
        private static readonly Interpreter interpreter = new();
        // This variable is used to indicate when an error has been encountered
        public static bool hadError = false;
        // This variable indicates if a runtime error has occured
        public static bool hadRuntimeError = false;

        /// <summary>
        /// Handles when and how to run jlox, either in line-by-line in the terminal or runnning a file specified by path.
        /// </summary>
        /// <param name="args">The command line arguments on which to run jlox. Optional specification of file name or path is allowed.</param>
        public static void Main(String[] args)
        {
            if (args.Length > 1)
            {
                Console.WriteLine("Usage: jlox [script]");
                Environment.Exit(64);
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
            if (hadError) Environment.Exit(65);
            if (hadRuntimeError) Environment.Exit(70);
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
        /// Runs the source code by using a scanner to scan in tokens and parsing the tokens into expressions.
        /// </summary>
        /// <param name="source">The file contents or line of code to be executed.</param>
        private static void Run(string source)
        {
            Scanner scanner = new(source);
            List<Token> tokens = scanner.ScanTokens();

            Parser parser = new(tokens);
            Expr? expression = parser.Parse(); // if Parse returns null, an error will be raised and the Print statement below will never execute.

            // Stop if there was a syntax error.
            if (hadError) return;

            interpreter.Interpret(expression);

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
        /// Calls <see cref="Report(int, string, string)"/> to report an error.
        /// </summary>
        /// <param name="token">Token that causes the error.</param>
        /// <param name="message">Error message to be printed</param>
        public static void Error(Token token, String message)
        {
            if (token.type == TokenType.EOF)
            {
                Report(token.line, " at end", message);
            }
            else
            {
                Report(token.line, " at '" + token.lexeme + "'", message);
            }
        }

        /// <summary>
        /// Reports a runtime error and indicates to the <see cref="Lox"/> class that an error has occured.
        /// </summary>
        /// <param name="error">Error thrown by interpreter.</param>
        public static void RuntimeError(RuntimeError error)
        {
            Console.Error.WriteLine(error.Message + "\n[line " + error.token.line + "]");
            hadRuntimeError = true;
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

