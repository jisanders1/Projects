using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static Lox_Interpreter.Lox.Expr;
using static Lox_Interpreter.Lox.Stmt;
using static System.Formats.Asn1.AsnWriter;

namespace Lox_Interpreter.Lox
{
    /// <summary>
    /// Represents a resolver that figures out which variables refer to what before interpreting.
    /// Ensures that the closure mechanism works properly.
    /// </summary>
    internal class Resolver : Expr.IVisitor<Object?>, Stmt.IVisitor<Object?>
    {
        private readonly Interpreter interpreter;

        // Keys: Environments, Booleans: whether or not a vairable has been resolved yet (true for resolved, false for not resolved)
        // only used by blocks
        private readonly Stack<Dictionary<String, bool>> scopes = new();

        // Indicates whether or not we are inside of a function or method
        private enum FunctionType
        {
            NONE,
            FUNCTION,
            INITIALIZER,
            METHOD
        }
        private FunctionType currentFunction = FunctionType.NONE;

        // Indicates whether or not we are inside a class
        private enum ClassType
        {
            NONE,
            CLASS
        }

        private ClassType currentClass = ClassType.NONE;

        /// <summary>
        /// Initializes an instance of the <see cref="Resolver"/> class with the specified interpreter.
        /// </summary>
        /// <param name="interpreter">The interpreter to be used after the resolver.</param>
        public Resolver(Interpreter interpreter)
        {
            this.interpreter = interpreter;
        }

        /// <summary>
        /// Resolves the variables for variable/assignment expressions within a block statement.
        /// </summary>
        /// <param name="stmt">Block statement to resolve.</param>
        /// <returns><see langword="null"/></returns>
        public Object? VisitBlockStmt(Block stmt)
        {
            BeginScope();
            Resolve(stmt.statements);
            EndScope();
            return null;
        }

        /// <summary>
        /// Resolves the variables for variable/assignment expressions within a class statement.
        /// </summary>
        /// <param name="stmt">Class statement to resolve.</param>
        /// <returns><see langword="null"/></returns>
        public Object? VisitClassStmt(Class stmt)
        {
            ClassType enclosingClass = currentClass; // Storing previous enclosing class in order to update current class.
            currentClass = ClassType.CLASS;

            Declare(stmt.name);
            Define(stmt.name);

            BeginScope();
            scopes.Peek()["this"] = true;

            foreach (Function method in stmt.methods)
            {
                FunctionType declaration = FunctionType.METHOD;
                if (method.name.lexeme.Equals("init"))
                {
                    declaration = FunctionType.INITIALIZER;
                }
                ResolveFunction(method, declaration);
            }

            currentClass = enclosingClass; // returning to previous enclosing class.

            EndScope();
            return null;
        }

        /// <summary>
        /// Resolves a function declaration statement.
        /// </summary>
        /// <param name="stmt">Function declaration statement to be resolved.</param>
        /// <returns><see langword="null"/></returns>
        public Object? VisitFunctionStmt(Function stmt)
        {
            // Function name is both declared and defined in the current scope.
            Declare(stmt.name);
            Define(stmt.name);

            ResolveFunction(stmt, FunctionType.FUNCTION); ;
            return null;
        }

        /// <summary>
        /// Adds an entry to the current innermost scope's dictionary.
        /// </summary>
        /// <param name="stmt">Variable statement to be resolved.</param>
        /// <returns><see langword="null"/></returns>
        public Object? VisitVarStmt(Var stmt)
        {
            Declare(stmt.name);
            if (stmt.initializer != null)
            {
                Resolve(stmt.initializer);
            }
            Define(stmt.name);
            return null;
        }

        /// <summary>
        /// Checks if a variable is present in its initializer before resolving it.
        /// </summary>
        /// <param name="expr">Variable expression to be resolved.</param>
        /// <returns><see langword="null"/></returns>
        public Object? VisitVariableExpr(Variable expr)
        {
            // added a third check to ensure the key is actually in the dictionary before attempting to check its value
            if (scopes.Count != 0 && scopes.Peek().TryGetValue(expr.name.lexeme, out bool isResolved) == true && isResolved == false) 
            {
                Lox.Error(expr.name, "Can't read local variable in its own initializer.");
            }

            ResolveLocal(expr, expr.name);
            return null;
        }

        /// <summary>
        /// Resolves an assignment expression.
        /// </summary>
        /// <param name="expr">Assignment expression to be resolved.</param>
        /// <returns><see langword="null"/></returns>
        public Object? VisitAssignExpr(Assign expr)
        {
            Resolve(expr.value); // Resolve expression first in case it references other variables.
            ResolveLocal(expr, expr.name); // Resolve assignment
            return null;
        }

        // Below 12 methods do not need to be resolved on their own, but they still needed to be implemented
        // in order to traverse the syntax tree

        public Object? VisitExpressionStmt(Expression stmt)
        {
            Resolve(stmt.expression);
            return null;
        }
        public Object? VisitIfStmt(If stmt)
        {
            Resolve(stmt.condition);
            Resolve(stmt.thenBranch);
            if (stmt.elseBranch != null) Resolve(stmt.elseBranch);
            return null;
        }
        public Object? VisitPrintStmt(Print stmt)
        {
            Resolve(stmt.expression);
            return null;
        }
        public Object? VisitReturnStmt(Stmt.Return stmt)
        {
            if (currentFunction == FunctionType.NONE) // Reports an error for attempting to return from top-level code.
            {
                Lox.Error(stmt.keyword, "Can't return from top-level code.");
            }
            if (stmt.value != null)
            {
                if (currentFunction == FunctionType.INITIALIZER) // Reports an error for attempting to return from an initializer
                {
                    Lox.Error(stmt.keyword, "Can't return a value from an initializer.");
                }
                Resolve(stmt.value);
            }

            return null;
        }
        public Object? VisitWhileStmt(While stmt)
        {
            Resolve(stmt.condition);
            Resolve(stmt.body);
            return null;
        }
        public Object? VisitBinaryExpr(Binary expr)
        {
            Resolve(expr.left);
            Resolve(expr.right);
            return null;
        }
        public Object? VisitCallExpr(Call expr)
        {
            Resolve(expr.callee);

            foreach (Expr argument in expr.arguments)
            {
                Resolve(argument);
            }

            return null;
        }
        public Object? VisitGetExpr(Get expr)
        {
            Resolve(expr.obj);
            return null;
        }
        public Object? VisitGroupingExpr(Grouping expr)
        {
            Resolve(expr.expression);
            return null;
        }
        public Object? VisitLiteralExpr(Literal expr)
        {
            return null;
        }
        public Object? VisitLogicalExpr(Logical expr)
        {
            Resolve(expr.left);
            Resolve(expr.right);
            return null;
        }
        public Object? VisitSetExpr(Set expr)
        {
            Resolve(expr.value);
            Resolve(expr.obj);
            return null;
        }
        public Object? VisitThisExpr(This expr)
        {
            if (currentClass == ClassType.NONE)
            {
                Lox.Error(expr.keyword, "Can't use 'this' outside of a class."); // Throws an error if the user attempts to use "this" outside of a class.
                return null;
            }
            ResolveLocal(expr, expr.keyword);
            return null;
        }
        public Object? VisitUnaryExpr(Unary expr)
        {
            Resolve(expr.right);
            return null;
        }

        /// <summary>
        /// Resolves the variables for variable/assignment expressions within a block statement.
        /// </summary>
        /// <param name="statements">List of statements to resolve.</param>
        public void Resolve(List<Stmt?> statements)
        {
            foreach (Stmt? statement in statements)
            {
                if (statement != null)
                {
                    Resolve(statement);
                }
            }
        }

        /// <summary>
        /// Helper function that resolves a statement.
        /// </summary>
        /// <param name="stmt">Statement to be resolved.</param>
        private void Resolve(Stmt stmt)
        {
            stmt.Accept(this);
        }

        /// <summary>
        /// Helper function that resolves an expression.
        /// </summary>
        /// <param name="expr">Expression to be resolved.</param>
        private void Resolve(Expr? expr)
        {
            expr?.Accept(this);
        }

        /// <summary>
        /// Walks through the scopes, innermost to outermost, looking the the specified name and expresion and resolves it if found.
        /// </summary>
        /// <param name="expr">Expression to search for</param>
        /// <param name="name">Expression's name to search for.</param>
        /// <remarks>The for loop was changed to starting at the end of the list and decrementing to starting at the beginning of the list and incrementing 
        /// due to an error caused by the introduction of the "this" keyword. The function would not recognize "this" as being in the dictionary although it certainly was.
        /// Incrementing fixed the issue, hopefully with no other side effects to running the program.
        /// </remarks>
        private void ResolveLocal(Expr expr, Token name)
        {
            List<Dictionary<String, bool>> list_scope = scopes.ToList(); // Converting to a list in order to iterate through the scopes.
            for (int i = 0; i < list_scope.Count; i++)
            {
                if (list_scope[i].ContainsKey(name.lexeme))
                {
                    interpreter.Resolve(expr, i); // Originally was list_scope.Count - 1 - i, had to adjust because using the original version caused errors later on in chapter 12
                    return;
                }
            }
        }

        /// <summary>
        /// Resolves a function's body and it's parameters within the functions scope. 
        /// Also updates what the enclosing function is so we know if we are inside a function or not.
        /// </summary>
        /// <param name="function">Function that needs resolving.</param>
        /// <param name="type">Current functions type.</param>
        private void ResolveFunction(Function function, FunctionType type)
        { 
            FunctionType enclosingFunction = currentFunction; // Stashing the previous enclosing function
            currentFunction = type; // Indicates we are now inside a new function
            
            BeginScope();
            foreach (Token param in function.parameters)
            {
                Declare(param);
                Define(param);
            }
            Resolve(function.body);
            EndScope();
            currentFunction = enclosingFunction; // restoring previous enclosing function's status
        }

        /// <summary>
        /// Starts a new scope as the innermost scope.
        /// </summary>
        private void BeginScope()
        {
            scopes.Push(new Dictionary<String, bool>());
        }

        /// <summary>
        /// Ends the innermost scope.
        /// </summary>
        private void EndScope()
        {
            scopes.Pop();
        }

        /// <summary>
        /// Declares a variable in the current innermost scope, but does not resolve it.
        /// </summary>
        /// <param name="name">Variable name to be declared.</param>
        private void Declare(Token name)
        {
            if (scopes.Count == 0) return;
            

            Dictionary<String, bool> scope = scopes.Peek();
            if (scope.ContainsKey(name.lexeme)) // Prevents user from declaring the same variable twice within the same scope.
            {
                Lox.Error(name, "Already a variable with this name in this scope."); 
            }

            scope[name.lexeme] = false;
        }

        /// <summary>
        /// Defines a variable as useable after the initializer has been resolved, resolves the variable as well.
        /// </summary>
        /// <param name="name">Variable to be defined and resolved.</param>
        private void Define(Token name)
        {
            if (scopes.Count == 0) return;
            scopes.Peek()[name.lexeme] = true;
        }
    }
}
