﻿using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static Lox_Interpreter.Lox.Expr;
using static Lox_Interpreter.Lox.Stmt;
using static Lox_Interpreter.Lox.TokenType;

namespace Lox_Interpreter.Lox
{
    /// <summary>
    /// Represents an interpreter that takes in a syntax tree and evaluates it.
    /// </summary>
    internal class Interpreter : Expr.IVisitor<Object?>, Stmt.IVisitor<Object?>
    {
        public readonly Environment globals = new(); // Global environment, contains native functions for the Lox language
        private Environment environment = new();// Base environment for the variables.
        /// <summary>
        /// Represents the native Lox function "clock()", which returns the current time in seconds.
        /// </summary>
        private class ClockFunction : ILoxCallable
        {
            public int Arity() 
            { 
                return 0;
            }
            public Object? Call(Interpreter interpreter, List<Object?> arguments) // In this case, returns the current time in seconds.
            {
                return (double)DateTime.UtcNow.Ticks / 10000.0 / 1000.0;
            }

            public override String ToString() 
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
                    if (statement == null) // not sure if this is the right way to handle this, will experiment to see what's up
                    {
                        continue;
                    }
                    Execute(statement);
                }
            }
            catch (RuntimeError error)
            {
                Lox.RuntimeError(error);
            }
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
        private Object? Evaluate(Expr expr)
        {
            return expr.Accept(this);
        }

        // Below 8 methods implement the Visitor paradigm for Statements.
        // Since void is not allowed as a generic type in C#, the return type is Object and null is simply returned.
        public Object? VisitVarStmt(Var stmt)
        {
            Object? value = null;
            if (stmt.initializer != null) // Evaluates the initializer if there is one.
            {
                value = Evaluate(stmt.initializer);
            }

            environment.Define(stmt.name.lexeme, value);
            return null;
        }
        public Object? VisitExpressionStmt(Expression stmt)
        {
            Evaluate(stmt.expression);
            return null;
        }
        public Object? VisitBlockStmt(Block stmt)
        {
            ExecuteBlock(stmt.statements, new Environment(environment)); // Evaluates a block of statements with their own scope.
            return null;
        }
        public Object? VisitPrintStmt(Print stmt)
        {
            Object? value = Evaluate(stmt.expression); // Evaluates the value of the print statement and prints it.
            Console.WriteLine(Stringify(value));
            return null;
        }
        public Object? VisitIfStmt(If stmt)
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
        public Object? VisitWhileStmt(While stmt)
        {
            while (IsTruthy(Evaluate(stmt.condition))) // Executes the body while the looping condition evaluates to true.
            {
                Execute(stmt.body);
            }
            return null;
        }
        public Object? VisitFunctionStmt(Function stmt)
        {
            LoxFunction function = new(stmt, environment); //declares a function in with its own environment
            environment.Define(stmt.name.lexeme, function); //adds it to base environment.
            return null;
        }
        public Object? VisitReturnStmt(Stmt.Return stmt)
        {
            Object? value = null;
            if (stmt.value != null) value = Evaluate(stmt.value); // returns an evaluated expression if the funciton body uses "return"

            throw new Return(value);
        }

        // Below 8 methods implement the Visitor paradigm through treating different expression ttypes differently.
        // Each method recursively calls Evaluate() except the Literal method, with simply returns it's value.
        // Otherwise, these do the actual evaluating.
        public Object? VisitAssignExpr(Assign expr)
        {
            Object? value = Evaluate(expr.value);
            environment.Assign(expr.name, value);
            return value;
        }
        public Object? VisitLiteralExpr(Literal expr)
        {
            return expr.value;
        }
        public Object? VisitGroupingExpr(Grouping expr)
        {
            return Evaluate(expr.expression);
        }
        public Object? VisitUnaryExpr(Unary expr)
        {
            Object? right = Evaluate(expr.right);

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
        public Object? VisitBinaryExpr(Binary expr)
        {
            Object? left = Evaluate(expr.left);
            Object? right = Evaluate(expr.right);

            // Below evaluates based on the type of operator.
            switch (expr.oper.type) {
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
                    if (left is double && right is double) {
                        return (double)left + (double)right;
                    }

                    if (left is string && right is string) {
                        return (string)left + (string)right;
                    }

                    throw new RuntimeError(expr.oper, "Operands must be two numbers or two strings.");
                case BANG_EQUAL: return !IsEqual(left, right); // !=
                case EQUAL_EQUAL: return IsEqual(left, right); // ==
            }
            
            // Unreachable.
            return null;
        }
        public Object? VisitVariableExpr(Variable expr)
        {
            return environment.Get(expr.name);
        }
        public Object? VisitLogicalExpr(Logical expr)
        {
            Object? left = Evaluate(expr.left);

            if (expr.oper.type == TokenType.OR) { // Allows for short-circuiting with the and/or operator
                if (IsTruthy(left)) return left;
            } else
            {
                if (!IsTruthy(left)) return left;
            }

            return Evaluate(expr.right);
        }
        public Object? VisitCallExpr(Call expr)
        {
            Object? callee = Evaluate(expr.callee); // Will normally evaluate to be an identifier of some sort, but not always.

            List<Object?> arguments = new();
            foreach (Expr argument in expr.arguments) // Evaluating the arguments
            {
                arguments.Add(Evaluate(argument));
            }

            if (!(callee is ILoxCallable)) { // Checks for improper use of the function syntax, i.e. attempting to use a string as a function name instead of an identifier.
                throw new RuntimeError(expr.paren,  "Can only call functions and classes.");
            }
            ILoxCallable? function = (ILoxCallable?)callee;
            if (arguments.Count != function?.Arity()) // Checks to make sure the number of arguments match whaat is expected.
            {
                throw new RuntimeError(expr.paren, "Expected " + function?.Arity() + " arguments but got " + arguments.Count + ".");
            }
            return function?.Call(this, arguments);
        }

        /// <summary>
        /// Verifies that a singular operand is a number. An exception is thrown when the operand is not a number.
        /// </summary>
        /// <param name="oper">Operator token of the expression.</param>
        /// <param name="operand">Operand being evaluated.</param>
        /// <exception cref="RuntimeError"></exception>
        private void CheckNumberOperand(Token oper, [NotNull] Object? operand)
        {
            if (operand is Double) return;
            throw new RuntimeError(oper, "Operand must be a number.");
        }

        /// <summary>
        /// Verifies that both the left and right operands are both numbers. Throws exception if either are not a number.
        /// </summary>
        /// <param name="oper">Operator token of the expression.</param> 
        /// <param name="left">Left operand of expression.</param>
        /// <param name="right">Right operand of expression.</param>
        /// <exception cref="RuntimeError"></exception>
        private void CheckNumberOperands(Token oper, [NotNull] Object? left, [NotNull] Object? right) //[NotNull] keyword indicates to C# that the statements won't be null upon returning from function.
        {
            if (left is double && right is double) return;

            throw new RuntimeError(oper, "Operands must be numbers.");
        }

        /// <summary>
        /// Handles how boolean values are treated in Lox.
        /// </summary>
        /// <param name="obj">Value to check for truthy-ness.</param>
        /// <returns><see langword="true"/> if the input is True or is a non-null value; otherwise, <see langword="false"/> if False or <see langword="null"/>.</returns>
        private bool IsTruthy(Object? obj)
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
        private bool IsEqual(Object? a, Object? b)
        {
            if (a == null && b == null) return true;
            if (a == null) return false;

            return a.Equals(b);
        }

        /// <summary>
        /// Converts an object to a string. All integers are treated as doubles in Lox, so the decimal is removed.
        /// </summary>
        /// <param name="obj">Object to be converted into a string.</param>
        /// <returns>A string equivalent of the object.</returns>
        private string? Stringify(Object? obj)
        {
            if (obj == null) return "nil";

            if (obj is double) {
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
