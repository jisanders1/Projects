using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static Lox_Interpreter.Lox.TokenType;
using static Lox_Interpreter.Lox.Expr;
using static Lox_Interpreter.Lox.Stmt;

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
        /// <summary>
        /// Initializes a new instance of the <see cref="Parser"/> class with a list of tokens.
        /// </summary>
        /// <param name="tokens">List of tokents to populate class's token field</param>
        public Parser(List<Token> tokens)
        {
            this.tokens = tokens;
        }

        /// <summary>
        /// Serves as an entry point to parsing the Lox language into executable statement with recursive descent. 
        /// </summary>
        /// <returns>A valid list of statements or expressions. This list may contain null values for errors.</returns>
        public List<Stmt?> Parse()
        {
            List<Stmt?> statements = new();
            while (!IsAtEnd())
            {
                statements.Add(Declaration());
            }

            return statements;
        }
        // Real parsing starts here
        /// <summary>
        /// Represents the rule for a declaration, which could be either an variable declaration or a statement. Catches a parsing error if one is thrown.
        /// </summary>
        /// <returns>A valid statement, variable declaration statement, or <see langword="null"/> if an error is caught.</returns>
        private Stmt? Declaration()
        {
            try
            {
                if (Match(VAR)) return VarDeclaration();

                return Statement();
            }
            catch (ParseError)
            {
                Synchronize();
                return null;
            }
        }

        /// <summary>
        /// Represents the syntax of a variable declaration.
        /// </summary>
        /// <returns>A variable statement with its own name and value.</returns>
        private Stmt VarDeclaration()
        {
            Token name = Consume(IDENTIFIER, "Expect variable name.");

            Expr? initializer = null;
            if (Match(EQUAL))
            {
                initializer = Expression();
            }

            Consume(SEMICOLON, "Expect ';' after variable declaration.");
            return new Var(name, initializer);
        }

        /// <summary>
        /// Represents the statement rule, which could be a print statement, a block, or an expression.
        /// </summary>
        /// <returns>A valid print, block, or expression statement.</returns>
        private Stmt Statement()
        {
            if (Match(PRINT)) return PrintStatement();
            if (Match(LEFT_BRACE)) return new Stmt.Block(Block());

            return ExpressionStatement();
        }

        /// <summary>
        /// Represents the syntax of a print statement. Checks for a valid expression and a semicolon.
        /// </summary>
        /// <returns>A valid print statement.</returns>
        private Stmt PrintStatement()
        {
            Expr value = Expression();
            Consume(SEMICOLON, "Expect ';' after value.");
            return new Print(value);
        }

        /// <summary>
        /// Represents the syntax of a valid expression statement (one that requires a semicolon at the end).
        /// </summary>
        /// <returns>A valid expression statement.</returns>
        private Stmt ExpressionStatement()
        {
            Expr expr = Expression();
            Consume(SEMICOLON, "Expect ';' after expression.");
            return new Expression(expr);
        }

        /// <summary>
        /// Repreents the syntax of a block statement, which could be an empty block or a block of declarations enclosed by {}.
        /// </summary>
        /// <returns>A list of valid declaration statements.</returns>
        private List<Stmt?> Block()
        {
            List<Stmt?> statements = new();

            while (!Check(RIGHT_BRACE) && !IsAtEnd())
            {
                statements.Add(Declaration());
            }

            Consume(RIGHT_BRACE, "Expect '}' after block.");
            return statements;
        }

        /// <summary>
        /// Represents the expression rule that consists of an assignment.
        /// </summary>
        /// <returns>A valid expression.</returns>
        private Expr Expression()
        {
            return Assignment();
        }

        /// <summary>
        /// Represents the rule for assignment. Treats the left-hand side as its own expression.
        /// </summary>
        /// <returns>A valid expression or assignment expression if appropriate.</returns>
        private Expr Assignment()
        {
            Expr expr = Equality();

            if (Match(EQUAL))
            {
                Token equals = Previous();
                Expr value = Assignment();

                if (expr is Variable) { //looks to see if the left-hand side of the expression is a variable, if so, it's assignment we do
                    Token name = ((Variable)expr).name;
                    return new Assign(name, value);
                }

                Error(equals, "Invalid assignment target.");
            }

            return expr;
        }


        /// <summary>
        /// Represents the equality rule and checks for the equality operators within an expression.
        /// </summary>
        /// <returns>A valid equality expression.</returns>
        private Expr Equality()
        {
            Expr expr = Comparison();

            while (Match(BANG_EQUAL, EQUAL_EQUAL))
            {
                Token oper = Previous();
                Expr right = Comparison();
                expr = new Binary(expr, oper, right);
            }

            return expr;
        }

        /// <summary>
        /// Represents the comparison rule and checks for comparison operators within an expression.
        /// </summary>
        /// <returns>A valid comparison expression.</returns>
        private Expr Comparison()
        {
            Expr expr = Term();

            while (Match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL))
            {
                Token oper = Previous();
                Expr right = Term();
                expr = new Binary(expr, oper, right);
            }

            return expr;
        }

        /// <summary>
        /// Represents the term rule, which checks for adding/subtracting.
        /// </summary>
        /// <returns>A valid term expression.</returns>
        private Expr Term()
        {
            Expr expr = Factor();

            while (Match(MINUS, PLUS))
            {
                Token oper = Previous();
                Expr right = Factor();
                expr = new Binary(expr, oper, right);
            }

            return expr;
        }

        /// <summary>
        /// Represents the factor rule, which checks for multiplication/division
        /// </summary>
        /// <returns>A valid factor expression.</returns>
        private Expr Factor()
        {
            Expr expr = Unary();

            while (Match(SLASH, STAR))
            {
                Token oper = Previous();
                Expr right = Unary();
                expr = new Binary(expr, oper, right);
            }

            return expr;
        }

        /// <summary>
        /// Represents the unary rule, which checks for the operators ! and negation.
        /// </summary>
        /// <returns>A valid unary expression.</returns>
        private Expr Unary()
        {
            if (Match(BANG, MINUS))
            {
                Token oper = Previous();
                Expr right = Unary();
                return new Unary(oper, right);
            }

            return Primary();
        }

        /// <summary>
        /// Represents the primary rule, which checks for literals and other terminals (such as nil). Will throw errors in cases of bad grouping use.
        /// </summary>
        /// <returns>A valid literal, or will throw an exception if problems in syntax are present.</returns>
        private Expr Primary()
        {
            if (Match(FALSE)) return new Literal(false);
            if (Match(TRUE)) return new Literal(true);
            if (Match(NIL)) return new Literal(null);

            if (Match(NUMBER, STRING))
            {
                return new Literal(Previous().literal);
            }

            if (Match(LEFT_PAREN))
            {
                Expr expr = Expression();
                Consume(RIGHT_PAREN, "Expect ')' after expression.");
                return new Grouping(expr);
            }

            if (Match(IDENTIFIER))
            {
                return new Variable(Previous());
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
        /// Continues advancing until the start of the nextstatement in order to restore a parser's state.
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
