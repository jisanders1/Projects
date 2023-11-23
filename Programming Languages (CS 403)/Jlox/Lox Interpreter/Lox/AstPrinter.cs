using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static Lox_Interpreter.Lox.Expr;

// This class can be deleted
namespace Lox_Interpreter.Lox
{
    /// <summary>
    /// Represents the printing of a syntax tree to the console.
    /// </summary>
    internal class AstPrinter : IVisitor<String>
    {
        /// <summary>
        /// Serves as a gateway for printing expressions. Exactly what is printed depends on the type of expression.
        /// </summary>
        /// <param name="expr">The expression needing to be printed.</param>
        /// <returns>String representation of the expression in the syntax tree.</returns>
        public string Print(Expr expr)
        {
            return expr.Accept(this);
        }

        public string VisitAssignExpr(Assign expr)
        {
            return "eh";
        }

        //The four functions below call the Parenthesize() function differently depending on the type of expression (binary, grouping, literal, unary).
        //These are called in the Expr class.
        public string VisitBinaryExpr(Binary expr)
        {
            return Parenthesize(expr.oper.lexeme, expr.left, expr.right);
        }

        public string VisitGroupingExpr(Grouping expr)
        {
            return Parenthesize("group", expr.expression);
        }

        public string VisitLiteralExpr(Literal expr)
        {
            if (expr.value == null) return "nil";
            string returnText = expr.value.ToString() ?? "nil";
            return returnText;
        }

        public string VisitUnaryExpr(Unary expr)
        {
            return Parenthesize(expr.oper.lexeme, expr.right);
        }

        public string VisitVariableExpr(Variable expr)
        {
            return "not dealing with this.";
        }

        /// <summary>
        /// Builds a new string to represent an expression(s) as an element of a syntax tree.
        /// </summary>
        /// <param name="name">The lexeme of the expression</param>
        /// <param name="exprs">Expression(s) to convert to strings.</param>
        /// <returns>A new string representation of the expression(s).</returns>
        private string Parenthesize(String name, params Expr[] exprs)
        {
            StringBuilder builder = new();

            builder.Append('(').Append(name);
            foreach (Expr expr in exprs)
            {
                builder.Append(' ');
                builder.Append(expr.Accept(this));
            }
            builder.Append(')');

            return builder.ToString();
        }
    }
}
