using Microsoft.VisualBasic;
using System;
using System.Linq;
using System.Collections.Generic;
using static Lox_Interpreter.Lox.TokenType;

namespace Lox_Interpreter.Lox
{
    /// <summary>
    /// Represents the scanning of source code into tokens.
    /// </summary>
    internal class Scanner
    {
        private readonly String source; // Source code
        private readonly List<Token> tokens = new(); //List of tokens
        private int start = 0; // indicates the start of a lexeme
        private int current = 0; // indicates the current position inside of source
        private int line = 1; // indicates the current line number
        private static readonly Dictionary<String, TokenType> keywords; // dictionary of key words

        /// <summary>
        /// Generates a dictionary of reserved words for the Lox language.
        /// </summary>
        static Scanner() 
        {
            keywords = new Dictionary<string, TokenType>
            {
                ["and"] = AND,
                ["class"] = CLASS,
                ["else"] = ELSE,
                ["false"] = FALSE,
                ["for"] = FOR,
                ["fun"] = FUN,
                ["if"] = IF,
                ["nil"] = NIL,
                ["or"] = OR,
                ["print"] = PRINT,
                ["return"] = RETURN,
                ["super"] = SUPER,
                ["this"] = THIS,
                ["true"] = TRUE,
                ["var"] = VAR,
                ["while"] = WHILE
            };
        }

        /// <summary>
        /// Initializes an instance of the <see cref="Scanner"/> class with the specified source.
        /// </summary>
        /// <param name="source">Source code to scan tokens from.</param>
        public Scanner(string source)
            {
                this.source = source;
            }

        /// <summary>
        /// Calls <see cref="ScanToken"/> while not at the end of the file in order to handle scnaning. Adds an EOF token at the end of the token list.
        /// </summary>
        /// <returns>A list of <see cref="Token"/>s with an EOF token at the end of the list.</returns>
        public List<Token> ScanTokens()
        {
            while (!IsAtEnd())
            {
                // We are at the beginning of the next lexeme.
                start = current;
                ScanToken();
            }
            tokens.Add(new Token(TokenType.EOF, "", null, line));
            return tokens;
        }

        /// <summary>
        /// Iterates through each character to determine the lexemes and tokens of the source code. Reports an error when an unrecognize character is read.
        /// </summary>
        private void ScanToken()
        {
            char c = Advance();
            switch (c)
            {
                // Single-character lexemes
                case '(': AddToken(LEFT_PAREN); break;
                case ')': AddToken(RIGHT_PAREN); break;
                case '{': AddToken(LEFT_BRACE); break;
                case '}': AddToken(RIGHT_BRACE); break;
                case ',': AddToken(COMMA); break;
                case '.': AddToken(DOT); break;
                case '-': AddToken(MINUS); break;
                case '+': AddToken(PLUS); break;
                case ';': AddToken(SEMICOLON); break;
                case '*': AddToken(STAR); break;

                // Double-character lexemes
                case '!':
                    AddToken(Match('=') ? BANG_EQUAL : BANG);
                    break;
                case '=':
                    AddToken(Match('=') ? EQUAL_EQUAL : EQUAL);
                    break;
                case '<':
                    AddToken(Match('=') ? LESS_EQUAL : LESS);
                    break;
                case '>':
                    AddToken(Match('=') ? GREATER_EQUAL : GREATER);
                    break;
                case '/':
                    if (Match('/'))
                    {
                        // A comment goes until the end of the line.
                        while (Peek() != '\n' && !IsAtEnd()) Advance();
                    }
                    else if (Match('*'))
                    {
                        MultiLineComment();
                    }
                    else
                    {
                        AddToken(SLASH);
                    }
                    break;

                // Whitespace
                case ' ':
                case '\r':
                case '\t':
                    // Ignore whitespace.
                    break;

                case '\n':
                    line++;
                    break;

                // Multi-character lexemes
                case '"': StringMethod(); break;
                default:
                    if (IsDigit(c))
                    {
                        Number();
                    }
                    else if (IsAlpha(c))
                    {
                        Identifier();
                    }
                    else
                    {
                        Lox.Error(line, "Unexpected character."); // Reports an error if the character is not alphanumeric.
                    }
                    break;
            }
        }

        /// <summary>
        /// Identifies the end of a multi-line comment and reports an error if the end is never reached.
        /// </summary>
        private void MultiLineComment()
        {
            while (!IsAtEnd())
            {
                char peek = Peek();
                if (peek == '*')
                {
                    current++;
                    if (Match('/')) return;
                }
                else if (peek == '\n')
                {
                    line++;
                }
                Advance();
            }

            if (IsAtEnd())
            {
                Lox.Error(line, "Unterminated comment.");
            }
        }

        /// <summary>
        /// Determines if a lexeme is an identifier or a reserved word of the language. 
        /// Once identified, it calls <see cref="AddToken(TokenType)"/> to add the lexeme to the token list.
        /// </summary>
        private void Identifier()
        {
            while (IsAlphaNumeric(Peek())) Advance(); //advacing to the end of the lexeme

            String text = source[start..current]; // Captures the full lexeme
            TokenType type;
            if (!keywords.ContainsKey(text)) // Checks if the lexeme is a keyword or not.
            {
                type = IDENTIFIER; // Lexeme is not a keyword.
            }
            else
            {
                type = keywords[text]; // Lexeme is a keyword.
            }
            AddToken(type);
        }

        /// <summary>
        /// Identifies a lexeme as a number and adds it as a token. All numbers are handled as floating-point decimals.
        /// </summary>
        private void Number()
        {
            while (IsDigit(Peek())) Advance(); // advances to the end of the number or to the decimal in a number

            // Look for a fractional part.
            if (Peek() == '.' && IsDigit(PeekNext()))
            {
                // Consume the "."
                Advance();

                while (IsDigit(Peek())) Advance();
            }

            AddToken(NUMBER, Double.Parse(source[start..current]));
        }

        /// <summary>
        /// Identifies a lexeme as a string and adds it as a token. Reports an error if the string is unterminated.
        /// </summary>
        private void StringMethod() {
            while (Peek() != '"' && !IsAtEnd())
            {
                if (Peek() == '\n') line++;
                Advance();
            }

            if (IsAtEnd()) {
                Lox.Error(line, "Unterminated string.");
                return;
            }

            // The closing ".
            Advance();

            // Trim the surrounding quotes.
            String value = source[(start + 1)..(current - 1)];
            AddToken(STRING, value);
        }

        /// <summary>
        /// Takes a character and compares it to the source at the current character. Iterates current if the character matches
        /// </summary>
        /// <param name="expected"></param>
        /// <returns><see langword="true"/> if the characters match; otherwise, <see langword="false"/> if at the end of the source or if the characters do not match.</returns>
        private bool Match(char expected)
        {
            if (IsAtEnd()) return false;
            if (source[current] != expected) return false;

            current++;
            return true;
        }

        /// <summary>
        /// Takes a peek at the current character and returns it.
        /// </summary>
        /// <returns>The null terminator if at the end of the list; otherwise, returns the current character in source.</returns>
        private char Peek()
        {
            if (IsAtEnd()) return '\0';
            return source[current];
        }

        /// <summary>
        /// Takes a peek at the next character and returns it.
        /// </summary>
        /// <returns>The null terminator if at the end of the list; otherwise, returns the next character in source.</returns>
        private char PeekNext()
        {
            if (current + 1 >= source.Length) return '\0';
            return source[current + 1];
        }

        /// <summary>
        /// Checks if a character is a digit between 0 and 9, inclusive.
        /// </summary>
        /// <param name="c">The character to check.</param>
        /// <returns><see langword="true"/> if the character is a digit between 0 and 9; otherwise, <see langword="false"/>.</returns>
        private bool IsDigit(char c)
        {
            return c >= '0' && c <= '9';
        }

        /// <summary>
        /// Checks if a character is a letter (A-Z or a-z) or underscore.
        /// </summary>
        /// <param name="c">The character to check.</param>
        /// <returns><see langword="true"/> if the character is a letter or underscore; otherwise <see langword="false"/>.</returns>
        private bool IsAlpha(char c)
        {
            return (c >= 'a' && c <= 'z') ||
                   (c >= 'A' && c <= 'Z') ||
                    c == '_';
        }

        /// <summary>
        /// Checks if a character is an alphanumeric character or an underscore.
        /// </summary>
        /// <param name="c">The character to check.</param>
        /// <returns><see langword="true"/> if the character is an alphanumeric charcter or an underscore; otherwise <see langword="false"/>.</returns>
        private bool IsAlphaNumeric(char c)
        {
            return IsAlpha(c) || IsDigit(c);
        }

        /// <summary>
        /// Checks if the <see cref="current"/> position is at the end of source.
        /// </summary>
        /// <returns><see langword="true"/> if <see cref="current"/> position is at the end of the source; otherwise <see langword="true"/>.</returns>
        private bool IsAtEnd()
        {
            return current >= source.Length;
        }

        /// <summary>
        /// Advances the <see cref="current"/> index pointer to the next index of the <see cref="source"/>.
        /// </summary>
        /// <returns>Original character that <see cref="current"/> pointed to before advancement.</returns>
        private char Advance()
        {
            return source[current++];
        }

        /// <summary>
        /// Calls <see cref="AddToken(TokenType, object?)"/> with null as the second argument to add a token to the <see cref="tokens"/> list.
        /// </summary>
        /// <param name="type">Type of token being added.</param>
        private void AddToken(TokenType type)
        {
            AddToken(type, null);
        }

        /// <summary>
        /// Adds a token to the <see cref="tokens"/> list with a type, the lexeme itself, literal, and line number.
        /// </summary>
        /// <param name="type">Type of token being added.</param>
        /// <param name="literal">Actual value of lexeme if applicable.</param>
        private void AddToken(TokenType type, Object? literal)
        {
            String text = source[start..current];
            tokens.Add(new Token(type, text, literal, line));
        }
    }
}
