using System.Text;

namespace Lox_Interpreter
{
    class Lox
    {
        ///this variable is used to indicate when an error has been encountered
        public static bool hadError = false;
        public static void Main(String[] args)
        {
            if (args.Length > 1)
            {
                Console.WriteLine("Usage: jlox [script]");
                System.Environment.Exit(64);
            }
            else if (args.Length == 1)
            {
                RunFile(args[0]);
            }
            else
            {
                RunPrompt();
            }
        }
        private static void RunFile(String path)
        {
            Encoding utf8 = new UTF8Encoding(true);
            byte[] bytes = File.ReadAllBytes(Path.GetFullPath(path));
            Run(new string(utf8.GetString(bytes)));
            if (hadError) System.Environment.Exit(65);
        }
        private static void RunPrompt()
        {
            using (StreamReader input = new StreamReader(Console.OpenStandardInput()))
            {
                string? line;
                Console.Write("> ");
                while ((line = input.ReadLine()) != null)
                {
                    Run(line);
                    hadError = false;
                    Console.Write("> ");
                }
            }
        }
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
        public static void Error(int line, String message)
        {
            Report(line, "", message);
        }

        private static void Report(int line, String where, String message)
        {
            Console.Error.WriteLine("[line " + line + "] Error" + where + ": " + message);
            hadError = true;
        }

    }
}

