using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static Lox_Interpreter.Lox.Expr;

namespace Lox_Interpreter.Lox
{
    class AstPrinter : IVisitor<String>
    {
        string Print(Expr expr)
        {
            return expr.Accept(this);
        }

        public String VisitBinaryExpr(Binary expr)
        {
            return Parenthesize(expr.oper.lexeme, expr.left, expr.right);
        }

        public String VisitGroupingExpr(Grouping expr)
        {
            return Parenthesize("group", expr.expression);
        }

        public String VisitLiteralExpr(Literal expr)
        {
            if (expr.value == null) return "nil";
            return expr.value.ToString();
        }

        public String VisitUnaryExpr(Unary expr)
        {
            return Parenthesize(expr.oper.lexeme, expr.right);
        }

        private String Parenthesize(String name, params Expr[] exprs)
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

        public static void Main(String[] args)
        {
            Expr expression = new Expr.Binary(
                new Expr.Unary(
                    new Token(TokenType.MINUS, "-", null, 1),
                    new Expr.Literal(123)),
                new Token(TokenType.STAR, "*", null, 1),
                new Expr.Grouping(
                    new Expr.Literal(45.67)));

            Console.WriteLine(new AstPrinter().Print(expression));
        }
    }
}
