using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static Lox_Interpreter.Lox.TokenType;

namespace Lox_Interpreter.Lox
{
    internal class Parser
    {
        /// <summary>
        /// Inherits from the <see cref="Exception"/> class in order to throw and report errors.
        /// </summary>
        private class ParseError : Exception { } // Static not allowed here, still testing whether SystemException or Exception works better
        private readonly List<Token> tokens; // List of tokens from Scanner
        private int current = 0; // Index representing the current token

        // Constructor
        public Parser(List<Token> tokens)
        {
            this.tokens = tokens;
        }

        /// <summary>
        /// Serves as an entry point to parsing expressions. Parses using recursive descent. 
        /// </summary>
        /// <returns>A valid expression, or null if there is a parsing error.</returns>
        public Expr? Parse()
        {
            try
            {
                return Expression();
            }
            catch (ParseError)
            {
                return null;
            }
        }

        /// <summary>
        /// Represents the expression rule. All other rules have higher precedence, so it calls <see cref="Expression"/>, the next highest in precedence.
        /// </summary>
        /// <returns>A valid expression, or an exception is thrown if a problem is found.</returns>
        private Expr Expression()
        {
            return Equality();
        }

        /// <summary>
        /// Represents the equality rule and checks for the equality operators. Calls <see cref="Comparison"/>, the next highest in precedence.
        /// </summary>
        /// <returns>A valid expression, or an exception is thrown if a problem is found.</returns>
        private Expr Equality()
        {
            Expr expr = Comparison();

            while (Match(BANG_EQUAL, EQUAL_EQUAL))
            {
                Token oper = Previous();
                Expr right = Comparison();
                expr = new Expr.Binary(expr, oper, right);
            }

            return expr;
        }

        /// <summary>
        /// Represents the comparison rule and checks for comparison operators (>, >=, <, <=). Calls <see cref="Term"/>, the next highest in precedence. 
        /// </summary>
        /// <returns>A valid expression, or an exception is thrown if a problem is found.</returns>
        private Expr Comparison()
        {
            Expr expr = Term();

            while (Match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL))
            {
                Token oper = Previous();
                Expr right = Term();
                expr = new Expr.Binary(expr, oper, right);
            }

            return expr;
        }

        /// <summary>
        /// Represents the term rule, which checks for adding/subtracting. Calls <see cref="Factor"/>, the next highest in precedence.
        /// </summary>
        /// <returns>A valid expression, or an exception is thrown if a problem is found.</returns>
        private Expr Term()
        {
            Expr expr = Factor();

            while (Match(MINUS, PLUS))
            {
                Token oper = Previous();
                Expr right = Factor();
                expr = new Expr.Binary(expr, oper, right);
            }

            return expr;
        }

        /// <summary>
        /// Represents the factor rule, which checks for multiplication/division. Calls <see cref="Unary"/>, the next highest in precedence. 
        /// </summary>
        /// <returns>A valid expression, or an exception is thrown if a problem is found.</returns>
        private Expr Factor()
        {
            Expr expr = Unary();

            while (Match(SLASH, STAR))
            {
                Token oper = Previous();
                Expr right = Unary();
                expr = new Expr.Binary(expr, oper, right);
            }

            return expr;
        }

        /// <summary>
        /// Represents the unary rule, which checks for the operators ! and negation. Calls <see cref="Primary"/>, the next highest in precedence. 
        /// </summary>
        /// <returns>A valid unary operator, or will throw an exception in case of errors.</returns>
        private Expr Unary()
        {
            if (Match(BANG, MINUS))
            {
                Token oper = Previous();
                Expr right = Unary();
                return new Expr.Unary(oper, right);
            }

            return Primary();
        }

        /// <summary>
        /// Represents the primary rule, which checks for literals and other terminals (such as nil). Will throw errors in cases of bad grouping use.
        /// </summary>
        /// <returns>A valid literal, or will throw an exception if problems in syntax are present.</returns>
        private Expr Primary()
        {
            if (Match(FALSE)) return new Expr.Literal(false);
            if (Match(TRUE)) return new Expr.Literal(true);
            if (Match(NIL)) return new Expr.Literal(null);

            if (Match(NUMBER, STRING))
            {
                return new Expr.Literal(Previous().literal);
            }

            if (Match(LEFT_PAREN))
            {
                Expr expr = Expression();
                Consume(RIGHT_PAREN, "Expect ')' after expression.");
                return new Expr.Grouping(expr);
            }

            throw Error(Peek(), "Expect expression.");
        }

        /// <summary>
        /// Checks if a number of types mach the current token.
        /// </summary>
        /// <param name="types"><see cref="TokenType"/>(s) to be checked for matches.</param>
        /// <returns><see langword="true"/> if the type(s) match the currenty token's type and advances to the next token; otherwise; <see langword="false"/>.</returns>
        private bool Match(params TokenType[] types)
        {
            foreach (TokenType type in types)
            {
                if (Check(type))
                {
                    Advance();
                    return true;
                }
            }

            return false;
        }

        /// <summary>
        /// Consumes tokens until the type matching the input type is found. Throws an exception if the input type is not found.
        /// </summary>
        /// <param name="type">Type to search for.</param>
        /// <param name="message">Error message to print in case the input type is not found.</param>
        /// <returns>The token matching the input's type if an exception is not thrown.'</returns>
        private Token Consume(TokenType type, String message)
        {
            if (Check(type)) return Advance();

            throw Error(Peek(), message);
        }

        /// <summary>
        /// Checks if the input's type is the same as the current token's type.
        /// </summary>
        /// <param name="type">The type to be checked.</param>
        /// <returns><see langword="true"/> if the input's type matches the current token's type; <see langword="false"/> if at the end of the token list or the types do not match.</returns>
        private bool Check(TokenType type)
        {
            if (IsAtEnd()) return false;
            return Peek().type == type;
        }

        /// <summary>
        /// Advances to the next token if not at the end of the token list.
        /// </summary>
        /// <returns>The original token before advancement.</returns>
        private Token Advance()
        {
            if (!IsAtEnd()) current++;
            return Previous();
        }

        /// <summary>
        /// Checks current position in token list.
        /// </summary>
        /// <returns><see langword="true"/> if at the end of the list (indicated by EOF <see cref="TokenType"/>); otherwise <see langword="false"/>.</returns>
        private bool IsAtEnd()
        {
            return Peek().type == EOF;
        }

        /// <summary>
        /// Returns the current token without advancing to the next token.
        /// </summary>
        /// <returns>The current token.</returns>
        private Token Peek()
        {
            return tokens[current];
        }

        /// <summary>
        /// Returns the previous token without advancing to the next toke.
        /// </summary>
        /// <returns>The previous token.</returns>
        private Token Previous()
        {
            return tokens[current - 1];
        }

        /// <summary>
        /// Reports to the <see cref="Lox"/> class that an error has occured.
        /// </summary>
        /// <param name="token">Token that caused the error.</param>
        /// <param name="message">Error message to be printed.</param>
        /// <returns>A <see cref="ParseError"/> exception.</returns>
        private ParseError Error(Token token, String message)
        {
            Lox.Error(token, message);
            return new ParseError();
        }

        /// <summary>
        /// Continues advancing until the end of a statement in order to restore a parser's state.
        /// </summary>
        private void Synchronize()
        {
            Advance();

            while (!IsAtEnd())
            {
                if (Previous().type == SEMICOLON) return;

                switch (Peek().type)
                {
                    case CLASS:
                    case FUN:
                    case VAR:
                    case FOR:
                    case IF:
                    case WHILE:
                    case PRINT:
                    case RETURN:
                        return;
                }

                Advance();
            }
        }
    }
}
