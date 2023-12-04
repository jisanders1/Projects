using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;
using static Lox_Interpreter.Stmt;

namespace Lox_Interpreter
{
    internal class LoxFunction : ILoxCallable
    {
        private readonly Function declaration; // contains the function's name, body, and parameters
        private readonly Environment closure; // for holding onto the environment after a function closes
        private readonly bool isInitializer; // used to indicate if a funciton is an initializer or a regular function
        public LoxFunction(Function declaration, Environment closure, bool isInitializer)
        {
            this.closure = closure;
            this.declaration = declaration;
            this.isInitializer = isInitializer;
        }
        public int Arity()
        {
            return declaration.parameters.Count;
        }
        public object? Call(Interpreter interpreter, List<object?> arguments)
        {
            Environment environment = new(closure);
            for (int i = 0; i < declaration.parameters.Count; i++)
            {
                environment.Define(declaration.parameters[i].lexeme, arguments[i]);
            }

            try
            {
                interpreter.ExecuteBlock(declaration.body, environment);
            }
            catch (Return returnValue) // catches a return value
            {
                if (isInitializer) return closure.GetAt(0, "this"); // establishes default return value of "this" for an initializer method
                return returnValue.value;
            }

            if (isInitializer) return closure.GetAt(0, "this");
            return null;
        }

        /// <summary>
        /// Binds the "this" keyword to the instance of its parent class.
        /// </summary>
        /// <param name="instance">Instance of class to bin "this" to.</param>
        /// <returns>A <see cref="LoxFunction"/> with "this" bound to it.</returns>
        public LoxFunction Bind(LoxInstance instance)
        {
            Environment environment = new(closure);
            environment.Define("this", instance);
            return new LoxFunction(declaration, environment, isInitializer);
        }
        public override string ToString()
        {
            return "<fn " + declaration.name.lexeme + ">";
        }
    }
}
