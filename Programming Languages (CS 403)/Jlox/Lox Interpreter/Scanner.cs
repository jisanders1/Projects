﻿using Microsoft.VisualBasic;
using System;
using System.Linq;
using static Lox_Interpreter.TokenType;

namespace Lox_Interpreter
{
    class Scanner
    {
        private readonly String source;
        private readonly List<Token> tokens = new();
        private int start = 0;
        private int current = 0;
        private int line = 1;
        private static readonly Dictionary<String, TokenType> keywords;

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
    public Scanner(string source)
        {
            this.source = source;
        }

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
        private void ScanToken()
        {
            char c = Advance();
            switch (c)
            {
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
                    else
                    {
                        AddToken(SLASH);
                    }
                    break;
                case ' ':
                case '\r':
                case '\t':
                    // Ignore whitespace.
                    break;

                case '\n':
                    line++;
                    break;
                case '"': StringMethod(); break;
                default:
                    if (Char.IsDigit(c)) // I chose to use C#'s implementation of checking for digits because it's concise and equivalent to the book.
                    {
                        Number();
                    }
                    else if (IsAlpha(c))
                    {
                        Identifier();
                    }
                    else
                    {
                        Lox.Error(line, "Unexpected character.");
                    }
                    break;
            }
        }
        private void Identifier()
        {
            while (IsAlphaNumeric(Peek())) Advance();

            String text = source[start..current];
            TokenType type;
            if (!keywords.ContainsKey(text))
            {
                type = IDENTIFIER;
            }
            else
            {
                type = keywords[text];
            }
            AddToken(type);
        }
        private void Number()
        {
            while (Char.IsDigit(Peek())) Advance();

            // Look for a fractional part.
            if (Peek() == '.' && Char.IsDigit(PeekNext()))
            {
                // Consume the "."
                Advance();

                while (Char.IsDigit(Peek())) Advance();
            }

            AddToken(NUMBER, Double.Parse(source[start..current]));
        }
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
        private bool Match(char expected)
        {
            if (IsAtEnd()) return false;
            if (source[current] != expected) return false;

            current++;
            return true;
        }
        private char Peek()
        {
            if (IsAtEnd()) return '\0';
            return source[current];
        }
        private char PeekNext()
        {
            if (current + 1 >= source.Length) return '\0';
            return source[current + 1];
        }
        private bool IsAlpha(char c)
        {
            return (c >= 'a' && c <= 'z') ||
                   (c >= 'A' && c <= 'Z') ||
                    c == '_';
        }
        private bool IsAlphaNumeric(char c)
        {
            return IsAlpha(c) || Char.IsDigit(c);
        }
        private bool IsAtEnd()
        {
            return current >= source.Length;
        }

        private char Advance()
        {
            return source[current++];
        }

        private void AddToken(TokenType type)
        {
            AddToken(type, null);
        }

        private void AddToken(TokenType type, Object? literal)
        {
            String text = source[start..current];
            tokens.Add(new Token(type, text, literal, line));
        }
    }
}
