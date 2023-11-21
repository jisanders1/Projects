using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static Lox_Interpreter.Lox.Expr;
using static Lox_Interpreter.Lox.TokenType;

namespace Lox_Interpreter.Lox
{
    /// <summary>
    /// Represents an interpreter that takes in a syntax tree and evaluates it.
    /// </summary>
    internal class Interpreter : IVisitor<Object>
    {
        /// <summary>
        /// Entry point to interpreter, catches and reports an error if one is thrown.
        /// </summary>
        /// <param name="expression">Syntax tree to be interpreted.</param>
        public void Interpret(Expr expression)
        {
            try
            {
                Object value = Evaluate(expression);
                Console.WriteLine(Stringify(value));
            }
            catch (RuntimeError error)
            {
                Lox.RuntimeError(error);
            }
        }

        /// <summary>
        /// Evaluates an expression through the Visior paradigm.
        /// </summary>
        /// <param name="expr">Expression to be evaluated.</param>
        /// <returns>An object containing the result of the evaluation.</returns>
        private Object Evaluate(Expr expr)
        {
            return expr.Accept(this);
        }

        // Below 4 methods implement the Visitor paradigm through treating different expression ttypes differently.
        // Each method recursively calls Evaluate() except the Literal method, with simply returns it's value.
        // Otherwise, these do the actual evaluating.
        public Object VisitLiteralExpr(Expr.Literal expr)
        {
            return expr.value;
        }
        public Object VisitGroupingExpr(Expr.Grouping expr)
        {
            return Evaluate(expr.expression);
        }
        public Object VisitUnaryExpr(Expr.Unary expr)
        {
            Object right = Evaluate(expr.right);

            switch (expr.oper.type) 
            {
                case BANG:
                    return !IsTruthy(right);
                case MINUS:
                    CheckNumberOperand(expr.oper, right);
                    return -(double)right;
            }

                // Unreachable.
            return null;
        }
        public Object VisitBinaryExpr(Binary expr)
        {
            Object left = Evaluate(expr.left);
            Object right = Evaluate(expr.right);

            switch (expr.oper.type) {
                case GREATER:
                    CheckNumberOperands(expr.oper, left, right);
                    return (double)left > (double)right;
                case GREATER_EQUAL:
                    CheckNumberOperands(expr.oper, left, right);
                    return (double)left >= (double)right;
                case LESS:
                    CheckNumberOperands(expr.oper, left, right);
                    return (double)left < (double)right;
                case LESS_EQUAL:
                    CheckNumberOperands(expr.oper, left, right);
                    return (double)left <= (double)right;
                case MINUS:
                    CheckNumberOperands(expr.oper, left, right);
                    return (double)left - (double)right;
                case SLASH:
                    CheckNumberOperands(expr.oper, left, right);
                    if ((double)right == 0.0) // Handles divide by 0 error.
                    {
                        throw new RuntimeError(expr.oper, "Cannot divide by zero.");
                    }
                    return (double)left / (double)right;
                case STAR:
                    CheckNumberOperands(expr.oper, left, right);
                    return (double)left * (double)right;
                case PLUS:
                    if (left is double && right is double) {
                        return (double)left + (double)right;
                    }

                    if (left is string && right is string) {
                        return (string)left + (string)right;
                    }

                    throw new RuntimeError(expr.oper, "Operands must be two numbers or two strings.");
                case BANG_EQUAL: return !IsEqual(left, right);
                case EQUAL_EQUAL: return IsEqual(left, right);
            }
            
            // Unreachable.
            return null;
        }

        /// <summary>
        /// Verifies that a singular operand is a number. An exception is thrown when the operand is not a number.
        /// </summary>
        /// <param name="oper">Operator token of the expression.</param>
        /// <param name="operand">Operand being evaluated.</param>
        /// <exception cref="RuntimeError"></exception>
        private void CheckNumberOperand(Token oper, Object operand)
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
        private void CheckNumberOperands(Token oper, Object left, Object right)
        {
            if (left is double && right is double) return;

            throw new RuntimeError(oper, "Operands must be numbers.");
        }

        /// <summary>
        /// Handles how boolean values are treated in Lox.
        /// </summary>
        /// <param name="obj">Value to check for truthy-ness.</param>
        /// <returns><see langword="true"/> if the input is True or is a non-null value; otherwise, <see langword="false"/> if False or <see langword="null"/>.</returns>
        private bool IsTruthy(Object obj)
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
        private bool IsEqual(Object a, Object b)
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
        private string Stringify(Object obj)
        {
            if (obj == null) return "nil";

            if (obj is double) {
                string text = obj.ToString();
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
