using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lox_Interpreter.Lox
{
    /// <summary>
    /// Rerpresents the environment of a program, which includes variable's and their values.
    /// </summary>
    internal class Environment
    {
        private readonly Environment? enclosing; // Parent environments.
        private readonly Dictionary<string, Object?> values = new(); // Stores variable names and values
        
        /// <summary>
        /// Initializes an instance of the <see cref="Environment"/> class with no parent.
        /// </summary>
        public Environment()
        {
            enclosing = null;
        }

        /// <summary>
        /// Initializes an instance of the <see cref="Environment"/> class with a parent environment.
        /// </summary>
        /// <param name="enclosing">The parent of the current environment.</param>
        public Environment(Environment enclosing) 
        {
            this.enclosing = enclosing;
        
        }

        /// <summary>
        /// Searches current and parent environment for given token (variable) and returns it if found; otherwise, an exception is thrown.
        /// </summary>
        /// <param name="name">Token being searched for</param>
        /// <returns>The value of the token stored in the environment.</returns>
        /// <exception cref="RuntimeError"></exception>
        public Object? Get(Token name)
        {
            if (values.ContainsKey(name.lexeme))
            {
                return values[name.lexeme];
            }

            if (enclosing != null) return enclosing.Get(name);

            throw new RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
        }

        /// <summary>
        /// Defines a new variable with the givem name and value in the current environment.
        /// </summary>
        /// <param name="name">Name of new variable.</param>
        /// <param name="value">Value of new variable.</param>
        public void Define(String name, Object? value)
        {
            values[name] = value;
        }

        /// <summary>
        /// Assigns a value to an existing variable. If the variable does not exist, an exception is thrown.
        /// </summary>
        /// <param name="name">Name of existing variable.</param>
        /// <param name="value">New value to be assigned to existing variable.</param>
        /// <exception cref="RuntimeError"></exception>
        public void Assign(Token name, Object? value)
        {
            if (values.ContainsKey(name.lexeme))
            {
                values[name.lexeme] = value;
                return;
            }

            if (enclosing != null)
            {
                enclosing.Assign(name, value);
                return;
            }

            throw new RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
        }
    }
}
