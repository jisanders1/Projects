using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static Lox_Interpreter.Expr;
using static Lox_Interpreter.Stmt;
using static Lox_Interpreter.TokenType;

namespace Lox_Interpreter
{
    /// <summary>
    /// Represents an interpreter that takes in a syntax tree and evaluates it.
    /// </summary>
    internal class Interpreter : Expr.IVisitor<object?>, Stmt.IVisitor<object?>
    {
        public readonly Environment globals = new(); // Global environment, contains native functions for the Lox language
        private Environment environment = new();// Base environment for the variables.
        private readonly Dictionary<Expr, int> locals = new();

        /// <summary>
        /// Represents the native Lox function "clock()", which returns the current time in seconds.
        /// </summary>
        private class ClockFunction : ILoxCallable
        {
            public int Arity()
            {
                return 0;
            }
            public object? Call(Interpreter interpreter, List<object?> arguments) // In this case, returns the current time in seconds.
            {
                return DateTime.UtcNow.Ticks / 10000.0 / 1000.0;
            }

            public override string ToString()
            {
                return "<native fn>";
            }
        }

        /// <summary>
        /// Initializes an instance of the <see cref="Interpreter"/> class with a global and base environment and a native <see cref="ClockFunction"/>.
        /// </summary>
        public Interpreter()
        {
            environment = globals; //Wrapping the base environment in the global variable
            globals.Define("clock", new ClockFunction());
        }


        /// <summary>
        /// Entry point to interpreter, catches and reports an runtime error if one is thrown.
        /// </summary>
        /// <param name="statements">Syntax tree to be interpreted.</param>
        public void Interpret(List<Stmt?> statements) //HANDLE POSSIBLE NULL STATEMENTS
        {
            try
            {
                foreach (Stmt? statement in statements)
                {
                    if (statement != null) // not sure if this is the right way to handle this, will experiment to see what's up
                    {
                        Execute(statement);
                    }

                }
            }
            catch (RuntimeError error)
            {
                Lox.RuntimeError(error);
            }
        }

        /// <summary>
        /// Stores resolution information in a side-table for when variables and assignments are executed.
        /// </summary>
        /// <param name="expr">Variable/assignment expression that has been resolved.</param>
        /// <param name="depth">Number of scopes between the current scope and the enclosing scope of the interpreter.</param>
        public void Resolve(Expr expr, int depth)
        {
            locals[expr] = depth;
        }

        /// <summary>
        /// Base call that executes a statement.
        /// </summary>
        /// <param name="stmt">Statement to be executed.</param>
        private void Execute(Stmt stmt)
        {
            stmt.Accept(this);
        }

        /// <summary>
        /// Executes a block of statements with their own <see cref="Environment"/>. After it is finished executing, it restores the base environment.
        /// </summary>
        /// <param name="statements">List of statements to execute.</param>
        /// <param name="environment">Environment for the scope of the block of statements.</param>
        public void ExecuteBlock(List<Stmt?> statements, Environment environment)
        {
            Environment previous = this.environment;
            try
            {
                this.environment = environment;

                foreach (Stmt? statement in statements)
                {
                    if (statement == null)
                    {
                        continue;
                    }
                    Execute(statement);
                }
            }
            finally
            {
                this.environment = previous;
            }
        }

        /// <summary>
        /// Evaluates an expression through the Visior paradigm.
        /// </summary>
        /// <param name="expr">Expression to be evaluated.</param>
        /// <returns>An object containing the result of the evaluation.</returns>
        private object? Evaluate(Expr expr)
        {
            return expr.Accept(this);
        }

        // Below 10 methods implement the Visitor paradigm for Statements.
        // Since void is not allowed as a generic type in C#, the return type is Object and null is simply returned.
        public object? VisitVarStmt(Var stmt)
        {
            object? value = null;
            if (stmt.initializer != null) // Evaluates the initializer if there is one.
            {
                value = Evaluate(stmt.initializer);
            }

            environment.Define(stmt.name.lexeme, value);
            return null;
        }
        public object? VisitExpressionStmt(Expression stmt)
        {
            Evaluate(stmt.expression);
            return null;
        }
        public object? VisitBlockStmt(Block stmt)
        {
            ExecuteBlock(stmt.statements, new Environment(environment)); // Evaluates a block of statements with their own scope.
            return null;
        }
        public object? VisitPrintStmt(Print stmt)
        {
            object? value = Evaluate(stmt.expression); // Evaluates the value of the print statement and prints it.
            Console.WriteLine(Stringify(value));
            return null;
        }
        public object? VisitIfStmt(If stmt)
        {
            if (IsTruthy(Evaluate(stmt.condition))) // Evaluates condition to see if it is true, if so, executes the then part of the statement.
            {
                Execute(stmt.thenBranch);
            }
            else if (stmt.elseBranch != null) // Executes the else branch (if there is one) if the condition evaluates to false.
            {
                Execute(stmt.elseBranch);
            }
            return null;
        }
        public object? VisitWhileStmt(While stmt)
        {
            while (IsTruthy(Evaluate(stmt.condition))) // Executes the body while the looping condition evaluates to true.
            {
                Execute(stmt.body);
            }
            return null;
        }
        public object? VisitFunctionStmt(Function stmt)
        {
            LoxFunction function = new(stmt, environment, false); //declares a function in with its own environment
            environment.Define(stmt.name.lexeme, function); //adds it to base environment.
            return null;
        }
        public object? VisitReturnStmt(Stmt.Return stmt)
        {
            object? value = null;
            if (stmt.value != null) value = Evaluate(stmt.value); // returns an evaluated expression if the funciton body uses "return"

            throw new Return(value);
        }
        public object? VisitClassStmt(Class stmt)
        {
            Object? superclass = null;
            if (stmt.superclass != null) // Checks to see if there is a superclass.
            {
                superclass = Evaluate(stmt.superclass);
                if (!(superclass is LoxClass)) { // Checks to ensure that superclass is indeed a class and not some other object (like a string, integer, etc.)
                    throw new RuntimeError(stmt.superclass.name, "Superclass must be a class.");
                }
            }
            environment.Define(stmt.name.lexeme, null);

            if (stmt.superclass != null) // Creates a new environment 
            {
                environment = new Environment(environment);
                environment.Define("super", superclass);
            }

            Dictionary<string, LoxFunction> methods = new();
            foreach (Function method in stmt.methods)
            {
                LoxFunction function = new(method, environment, method.name.lexeme.Equals("init"));
                methods[method.name.lexeme] = function;
            }

            LoxClass klass = new(stmt.name.lexeme, (LoxClass?)superclass, methods);

            if (superclass != null && environment.enclosing != null)
            {
                environment = environment.enclosing;
            }

            environment.Assign(stmt.name, klass);
            return null;
        }

        // Below 11 methods implement the Visitor paradigm through treating different expression ttypes differently.
        // Each method recursively calls Evaluate() except the Literal method, with simply returns it's value.
        // Otherwise, these do the actual evaluating.
        public object? VisitAssignExpr(Assign expr)
        {
            object? value = Evaluate(expr.value);
            if (locals.TryGetValue(expr, out int distance))
            {
                environment.AssignAt(distance, expr.name, value);
            }
            else
            {
                globals.Assign(expr.name, value);
            }
            return value;
        }
        public object? VisitLiteralExpr(Literal expr)
        {
            return expr.value;
        }
        public object? VisitGroupingExpr(Grouping expr)
        {
            return Evaluate(expr.expression);
        }
        public object? VisitUnaryExpr(Unary expr)
        {
            object? right = Evaluate(expr.right);

            switch (expr.oper.type)
            {
                case BANG: // Negation of boolean values.
                    return !IsTruthy(right);
                case MINUS: // Negation
                    CheckNumberOperand(expr.oper, right);
                    return -(double)right;
            }

            // Unreachable.
            return null;
        }
        public object? VisitBinaryExpr(Binary expr)
        {
            object? left = Evaluate(expr.left);
            object? right = Evaluate(expr.right);

            // Below evaluates based on the type of operator.
            switch (expr.oper.type)
            {
                case GREATER: // >
                    CheckNumberOperands(expr.oper, left, right);
                    return (double)left > (double)right;
                case GREATER_EQUAL: // >=
                    CheckNumberOperands(expr.oper, left, right);
                    return (double)left >= (double)right;
                case LESS: // <
                    CheckNumberOperands(expr.oper, left, right);
                    return (double)left < (double)right;
                case LESS_EQUAL: // <=
                    CheckNumberOperands(expr.oper, left, right);
                    return (double)left <= (double)right;
                case MINUS: // - (subtraction)
                    CheckNumberOperands(expr.oper, left, right);
                    return (double)left - (double)right;
                case SLASH:// / (division)
                    CheckNumberOperands(expr.oper, left, right);
                    if ((double)right == 0.0) // Handles divide by 0 error.
                    {
                        throw new RuntimeError(expr.oper, "Cannot divide by zero.");
                    }
                    return (double)left / (double)right;
                case STAR: // * (multiplication)
                    CheckNumberOperands(expr.oper, left, right);
                    return (double)left * (double)right;
                case PLUS: // + (addition)
                    if (left is double && right is double)
                    {
                        return (double)left + (double)right;
                    }

                    if (left is string && right is string)
                    {
                        return (string)left + (string)right;
                    }

                    throw new RuntimeError(expr.oper, "Operands must be two numbers or two strings.");
                case BANG_EQUAL: return !IsEqual(left, right); // !=
                case EQUAL_EQUAL: return IsEqual(left, right); // ==
            }

            // Unreachable.
            return null;
        }
        public object? VisitVariableExpr(Variable expr)
        {
            return LookUpVariable(expr.name, expr);
        }
        public object? VisitLogicalExpr(Logical expr)
        {
            object? left = Evaluate(expr.left);

            if (expr.oper.type == OR)
            { // Allows for short-circuiting with the and/or operator
                if (IsTruthy(left)) return left;
            }
            else
            {
                if (!IsTruthy(left)) return left;
            }

            return Evaluate(expr.right);
        }
        public object? VisitCallExpr(Call expr)
        {
            object? callee = Evaluate(expr.callee); // Will normally evaluate to be an identifier of some sort, but not always.

            List<object?> arguments = new();
            foreach (Expr argument in expr.arguments) // Evaluating the arguments
            {
                arguments.Add(Evaluate(argument));
            }

            if (!(callee is ILoxCallable))
            { // Checks for improper use of the function syntax, i.e. attempting to use a string as a function name instead of an identifier.
                throw new RuntimeError(expr.paren, "Can only call functions and classes.");
            }
            ILoxCallable? function = (ILoxCallable?)callee;
            if (arguments.Count != function?.Arity()) // Checks to make sure the number of arguments match whaat is expected.
            {
                throw new RuntimeError(expr.paren, "Expected " + function?.Arity() + " arguments but got " + arguments.Count + ".");
            }
            return function?.Call(this, arguments);
        }
        public object? VisitGetExpr(Get expr)
        {
            object? obj = Evaluate(expr.obj);
            if (obj is LoxInstance)
            {
                return ((LoxInstance)obj).Get(expr.name);
            }

            throw new RuntimeError(expr.name, "Only instances have properties."); // Throws an exception for attempting to access a property of something that is not a class.
        }
        public object? VisitSetExpr(Set expr)
        {
            object? obj = Evaluate(expr.obj);

            if (!(obj is LoxInstance))
            {
                throw new RuntimeError(expr.name, "Only instances have fields."); // Throws an exception for attempting to access a field of something that is not a class.
            }

            object? value = Evaluate(expr.value);
            ((LoxInstance)obj).Set(expr.name, value);
            return value;
        }
        public object? VisitSuperExpr(Super expr)
        {
            int distance = locals[expr]; // If we have gotten to this point, the superclass must be in the locals table.
            LoxClass? superclass = (LoxClass?)environment.GetAt(distance, "super");

            // An error should never be thrown with this, simply handles a warning.
            LoxInstance? obj = (LoxInstance?)environment.GetAt(distance - 1, "this") ?? throw new RuntimeError(expr.method, "Instance of class not found.");

            // Using coalesce operator to throw an exception if method is null
            LoxFunction? method = (superclass?.FindMethod(expr.method.lexeme)) ?? throw new RuntimeError(expr.method, "Undefined property '" + expr.method.lexeme + "'.");
            return method.Bind(obj);
        }
        public object? VisitThisExpr(This expr)
        {
            return LookUpVariable(expr.keyword, expr);
        }

        /// <summary>
        /// Verifies that a singular operand is a number. An exception is thrown when the operand is not a number.
        /// </summary>
        /// <param name="oper">Operator token of the expression.</param>
        /// <param name="operand">Operand being evaluated.</param>
        /// <exception cref="RuntimeError"></exception>
        private void CheckNumberOperand(Token oper, [NotNull] object? operand)
        {
            if (operand is double) return;
            throw new RuntimeError(oper, "Operand must be a number.");
        }

        /// <summary>
        /// Verifies that both the left and right operands are both numbers. Throws exception if either are not a number.
        /// </summary>
        /// <param name="oper">Operator token of the expression.</param> 
        /// <param name="left">Left operand of expression.</param>
        /// <param name="right">Right operand of expression.</param>
        /// <exception cref="RuntimeError"></exception>
        private void CheckNumberOperands(Token oper, [NotNull] object? left, [NotNull] object? right) //[NotNull] keyword indicates to C# that the statements won't be null upon returning from function.
        {
            if (left is double && right is double) return;

            throw new RuntimeError(oper, "Operands must be numbers.");
        }

        /// <summary>
        /// Handles how boolean values are treated in Lox.
        /// </summary>
        /// <param name="obj">Value to check for truthy-ness.</param>
        /// <returns><see langword="true"/> if the input is True or is a non-null value; otherwise, <see langword="false"/> if False or <see langword="null"/>.</returns>
        private bool IsTruthy(object? obj)
        {
            if (obj == null) return false;
            if (obj is bool) return (bool)obj;
            return true;
        }

        /// <summary>
        /// Handles what is treated as equal.
        /// </summary>
        /// <param name="a">Left operand.</param>
        /// <param name="b">Right operand</param>
        /// <returns><see langword="true"/> if a and b are equivalent objects or if a and b are null, otherwise, <see langword="false"/>.</returns>
        private bool IsEqual(object? a, object? b)
        {
            if (a == null && b == null) return true;
            if (a == null) return false;

            return a.Equals(b);
        }

        /// <summary>
        /// Looks for a variable, first in the locals scope, then within the global scope. 
        /// </summary>
        /// <param name="name">Name of variable to be searched.</param>
        /// <param name="expr">Expression that contains the variable.</param>
        /// <returns>The variable's value if found.</returns>
        private object? LookUpVariable(Token name, Expr expr)
        {
            if (locals.TryGetValue(expr, out int distance))
            {
                return environment.GetAt(distance, name.lexeme);
            }
            else
            {
                return globals.Get(name);
            }
        }

        /// <summary>
        /// Converts an object to a string. All integers are treated as doubles in Lox, so the decimal is removed.
        /// </summary>
        /// <param name="obj">Object to be converted into a string.</param>
        /// <returns>A string equivalent of the object.</returns>
        private string? Stringify(object? obj)
        {
            if (obj == null) return "nil";

            if (obj is double)
            {
                // "nil" will never be used here since obj is not null
                // It is simply here to handle a possible null reference warning with the null-coalescing operator
                string text = obj.ToString() ?? "nil";
                if (text.EndsWith(".0")) // Accounting for the case of an integer
                {
                    text = text[..^2];
                }
                return text;
            }

            return obj.ToString();
        }
    }
}
