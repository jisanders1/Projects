using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;
using static Lox_Interpreter.Lox.Stmt;

namespace Lox_Interpreter.Lox
{
    internal class LoxFunction : ILoxCallable
    {
        private readonly Function declaration; // contains the function's name, body, and parameters
        private readonly Environment closure; // for holding onto the environment after a function closes
        public LoxFunction(Function declaration, Environment closure)
        {
            this.closure = closure;
            this.declaration = declaration;
        }
        public int Arity()
        {
            return declaration.parameters.Count;
        }
        public Object? Call(Interpreter interpreter, List<Object?> arguments)
        {
            Environment environment = new(closure);
            for (int i = 0; i < declaration.parameters.Count; i++) {
                environment.Define(declaration.parameters[i].lexeme, arguments[i]);
            }

            try
            {
                interpreter.ExecuteBlock(declaration.body, environment);
            }
            catch (Return returnValue)
            {
                return returnValue.value;
            }
            return null;
        }
        public override String ToString()
        {
            return "<fn " + declaration.name.lexeme + ">";
        }
    }
}
