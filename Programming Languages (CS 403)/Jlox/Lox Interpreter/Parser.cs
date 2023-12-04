using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static Lox_Interpreter.TokenType;
using static Lox_Interpreter.Expr;
using static Lox_Interpreter.Stmt;

namespace Lox_Interpreter
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
        /// Represents the rule for a declaration, which could be either an variable/function declaration or a statement. Catches a parsing error if one is thrown.
        /// </summary>
        /// <returns>A valid statement, variable declaration statement, or <see langword="null"/> if an error is caught.</returns>
        private Stmt? Declaration()
        {
            try
            {
                if (Match(CLASS)) return ClassDeclaration();
                if (Match(FUN)) return Function("function");
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
        /// Represents the syntax of a functino declaration (i.e. fun foo(x,y) { statements }).
        /// </summary>
        /// <param name="kind">Type of name expected, can be "function" or "method" for classes.</param>
        /// <returns>A valid function statement.</returns>
        private Function Function(string kind)
        {
            Token name = Consume(IDENTIFIER, "Expect " + kind + " name."); // Throws an error if no name is present after "fun"
            Consume(LEFT_PAREN, "Expect '(' after " + kind + " name."); // Throws an error if there is no '(' before the parameters
            List<Token> parameters = new();
            if (!Check(RIGHT_PAREN))
            {
                do
                {
                    if (parameters.Count >= 255) // Limits number of paramters to 255
                    {
                        Error(Peek(), "Can't have more than 255 parameters.");
                    }

                    parameters.Add(Consume(IDENTIFIER, "Expect parameter name."));
                } while (Match(COMMA)); // Keeps adding parameters while there are commas present.
            }
            Consume(RIGHT_PAREN, "Expect ')' after parameters."); // Throws error is there is no ')' present after the parameters.
            Consume(LEFT_BRACE, "Expect '{' before " + kind + " body."); // Throws error if there is no '{' for the body of the function
            List<Stmt?> body = Block();
            return new Function(name, parameters, body);
        }

        /// <summary>
        /// Represents the syntax of a class declaration.
        /// </summary>
        /// <returns>A class declaration statement.</returns>
        private Stmt ClassDeclaration()
        {
            Token name = Consume(IDENTIFIER, "Expect class name."); // Throws an exception if a name is not found
            
            Variable? superclass = null;
            if (Match(LESS))
            {
                Consume(IDENTIFIER, "Expect superclass name.");
                superclass = new Variable(Previous());
            }
            
            Consume(LEFT_BRACE, "Expect '{' before class body."); // Throws an exception if the opening curly brace is missing.

            List<Function> methods = new();
            while (!Check(RIGHT_BRACE) && !IsAtEnd()) // Adds new functions while within the class declaration.
            {
                methods.Add(Function("method"));
            }

            Consume(RIGHT_BRACE, "Expect '}' after class body."); // Throws an exception if the closing curly brace is mising.

            return new Class(name, superclass, methods);
        }
        /// <summary>
        /// Represents the syntax of a variable declaration.
        /// </summary>
        /// <returns>A variable statement with its own name and value.</returns>
        private Stmt VarDeclaration()
        {
            Token name = Consume(IDENTIFIER, "Expect variable name."); // Throws error if variable is not given a name

            Expr? initializer = null;
            if (Match(EQUAL)) // Allows for a simple declaration with no initializer if necessary.
            {
                initializer = Expression();
            }

            Consume(SEMICOLON, "Expect ';' after variable declaration."); // Throws error if there is no trailing semicolon.
            return new Var(name, initializer);
        }

        /// <summary>
        /// Represents the statement rule, which could be a print, a block, an expression, a for, a while, an if, or a return statement.
        /// </summary>
        /// <returns>A valid print, block, expression, for, while, if, or return statement.</returns>
        private Stmt Statement()
        {
            if (Match(FOR)) return ForStatement();
            if (Match(IF)) return IfStatement();
            if (Match(PRINT)) return PrintStatement();
            if (Match(RETURN)) return ReturnStatement();
            if (Match(WHILE)) return WhileStatement();
            if (Match(LEFT_BRACE)) return new Block(Block());

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
        /// Rerpresents the syntax of a valid if statement, similar to the C language.
        /// </summary>
        /// <returns>A valid if statement.</returns>
        private Stmt IfStatement()
        {
            Consume(LEFT_PAREN, "Expect '(' after 'if'.");
            Expr condition = Expression();
            Consume(RIGHT_PAREN, "Expect ')' after if condition.");

            Stmt thenBranch = Statement();
            Stmt? elseBranch = null;
            if (Match(ELSE))
            {
                elseBranch = Statement();
            }

            return new If(condition, thenBranch, elseBranch);
        }

        /// <summary>
        /// Represents the sytanx of a valid while loop, similar to the C language.
        /// </summary>
        /// <returns>A valid while loop.</returns>
        private Stmt WhileStatement()
        {
            Consume(LEFT_PAREN, "Expect '(' after 'while'.");
            Expr condition = Expression();
            Consume(RIGHT_PAREN, "Expect ')' after condition.");
            Stmt body = Statement();

            return new While(condition, body);
        }

        /// <summary>
        /// Reresents the syntax of a valid for loop, similar to the C language.
        /// </summary>
        /// <returns>A valid for loop.</returns>
        private Stmt ForStatement()
        {
            Consume(LEFT_PAREN, "Expect '(' after 'for'.");

            // Initializer portion of loop
            Stmt? initializer;
            if (Match(SEMICOLON))
            {
                initializer = null;
            }
            else if (Match(VAR))
            {
                initializer = VarDeclaration();
            }
            else
            {
                initializer = ExpressionStatement();
            }

            // Condition portion of loop
            Expr? condition = null;
            if (!Check(SEMICOLON))
            {
                condition = Expression();
            }
            Consume(SEMICOLON, "Expect ';' after loop condition.");

            // Increment portion of loop
            Expr? increment = null;
            if (!Check(RIGHT_PAREN))
            {
                increment = Expression();
            }
            Consume(RIGHT_PAREN, "Expect ')' after for clauses.");
            Stmt body = Statement();

            // Representing a for loop with a while loop in order to avoid additional methods/class changes.
            if (increment != null)
            {
                body = new Block(new List<Stmt?> { body, new Expression(increment) });
            }

            if (condition == null) condition = new Literal(true); // Default to true if no condition is provided.
            body = new While(condition, body);

            if (initializer != null) // Ran only once before the body executes.
            {
                body = new Block(new List<Stmt?> { initializer, body });
            }
            return body;
        }

        /// <summary>
        /// Represents the syntax of a return statement.
        /// </summary>
        /// <returns>A valid return statement with the expression parsed if there is one.</returns>
        private Stmt ReturnStatement()
        {
            Token keyword = Previous(); // consumes the "return" keyword
            Expr? value = null;
            if (!Check(SEMICOLON)) // Evaluates the expression if there is one before the semicolon.
            {
                value = Expression();
            }

            Consume(SEMICOLON, "Expect ';' after return value."); // Throws an error if there is no semicolon at the end of the statement.
            return new Stmt.Return(keyword, value);
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
            Expr expr = Or();

            if (Match(EQUAL))
            {
                Token equals = Previous();
                Expr value = Assignment();

                if (expr is Variable)
                { //looks to see if the left-hand side of the expression is a variable, if so, it's assignment we do
                    Token name = ((Variable)expr).name;
                    return new Assign(name, value);
                }
                else if (expr is Get)
                {
                    Get get = (Get)expr;
                    return new Set(get.obj, get.name, value);
                }

                Error(equals, "Invalid assignment target.");
            }

            return expr;
        }

        /// <summary>
        /// Reresents the syntax to an or opertor, which is similar to a binary operator with special condiitions.
        /// </summary>
        /// <returns>A valid or operator, or a valid and operator.</returns>
        private Expr Or()
        {
            Expr expr = And();

            while (Match(OR))
            {
                Token oper = Previous();
                Expr right = And();
                expr = new Logical(expr, oper, right);
            }

            return expr;
        }

        /// <summary>
        /// Represents the syntax to an and operator, which is similar to a binary operator with special conditions.
        /// </summary>
        /// <returns>A valid or operator, or an equality expression.</returns>
        private Expr And()
        {
            Expr expr = Equality();

            while (Match(AND))
            {
                Token oper = Previous();
                Expr right = Equality();
                expr = new Logical(expr, oper, right);
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

            return Call();
        }

        /// <summary>
        /// Represents the syntax of a function call or class accessing (methods and properties)
        /// </summary>
        /// <returns>A valid function call expression.</returns>
        private Expr Call()
        {
            Expr expr = Primary(); // evaluates the function name

            while (true)
            {
                if (Match(LEFT_PAREN)) // parses the arguments of the function
                {
                    expr = FinishCall(expr);
                }
                else if (Match(DOT)) // parses class property accesses
                {
                    Token name = Consume(IDENTIFIER, "Expect property name after '.'.");
                    expr = new Get(expr, name);
                }
                else
                {
                    break;
                }
            }

            return expr;
        }

        /// <summary>
        /// Helper function to <see cref="Call"/>, parses up to 255 arguments of a function call.
        /// </summary>
        /// <param name="callee">Function whose arguments we are parsing.</param>
        /// <returns>A valid function call expression with arguments.</returns>
        private Expr FinishCall(Expr callee)
        {
            List<Expr> arguments = new();
            if (!Check(RIGHT_PAREN))
            {
                do
                {
                    if (arguments.Count >= 255)
                    {
                        Error(Peek(), "Can't have more than 255 arguments."); // Throws an exception if there are more than 255 arguments.
                    }
                    arguments.Add(Expression());
                } while (Match(COMMA)); // Loops while there are still commas.
            }

            Token paren = Consume(RIGHT_PAREN, "Expect ')' after arguments."); // Throws an expetion if there is no closing ')'.

            return new Call(callee, paren, arguments);
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

            if (Match(LEFT_PAREN)) // Parses grouped expressions (those that have parenthesis)
            {
                Expr expr = Expression();
                Consume(RIGHT_PAREN, "Expect ')' after expression."); // Throws an excption if there is no closing ')'
                return new Grouping(expr);
            }

            if (Match(SUPER))
            {
                Token keyword = Previous();
                Consume(DOT, "Expect '.' after 'super'.");
                Token method = Consume(IDENTIFIER, "Expect superclass method name.");
                return new Super(keyword, method);
            }

            if (Match(THIS)) return new This(Previous()); // Parses this keyword

            if (Match(IDENTIFIER)) // Parses variables and function names.
            {
                return new Variable(Previous());
            }

            throw Error(Peek(), "Expect expression."); // Throws an exception if an invalid expression is present.
        }

        /// <summary>
        /// Checks if a number of types mach the current token.
        /// </summary>
        /// <param name="types"><see cref="TokenType"/>(s) to be checked for matches.</param>
        /// <returns><see langword="true"/> if the type(s) match the currents token's type and advances to the next token; otherwise; <see langword="false"/>.</returns>
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
        private Token Consume(TokenType type, string message)
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
        private ParseError Error(Token token, string message)
        {
            Lox.Error(token, message);
            return new ParseError();
        }

        /// <summary>
        /// Continues advancing until the start of the next statement in order to restore a parser's state.
        /// </summary>
        private void Synchronize()
        {
            Advance();

            while (!IsAtEnd()) // advances while not at the en of the token list.
            {
                if (Previous().type == SEMICOLON) return; // returns after the end of a statement

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
                        return; // returns if current type is a Lox keyword.
                }

                Advance();
            }
        }
    }
}
