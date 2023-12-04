using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lox_Interpreter
{
    /// <summary>
    /// Rerpresents the environment of a program, which includes variable's and their values.
    /// </summary>
    internal class Environment
    {
        public readonly Environment? enclosing; // Parent environments.
        private readonly Dictionary<string, object?> values = new(); // Stores variable names and values

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
        public object? Get(Token name)
        {
            if (values.ContainsKey(name.lexeme))
            {
                return values[name.lexeme];
            }

            if (enclosing != null) return enclosing.Get(name);

            throw new RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
        }

        /// <summary>
        /// Retrieves a variable's value by stepping through a certain known number of scopes.
        /// </summary>
        /// <param name="distance">Number of steps between current environment and variable's environment.</param>
        /// <param name="name">Name of variable being searched for.</param>
        /// <returns>Value of variable stored in the ancestors environment.</returns>
        public object? GetAt(int distance, string name)
        {
            return Ancestor(distance).values[name];
        }



        /// <summary>
        /// Defines a new variable with the givem name and value in the current environment.
        /// </summary>
        /// <param name="name">Name of new variable.</param>
        /// <param name="value">Value of new variable.</param>
        public void Define(string name, object? value)
        {
            values[name] = value;
        }

        /// <summary>
        /// Assigns a value to an existing variable. If the variable does not exist, an exception is thrown.
        /// </summary>
        /// <param name="name">Name of existing variable.</param>
        /// <param name="value">New value to be assigned to existing variable.</param>
        /// <exception cref="RuntimeError"></exception>
        public void Assign(Token name, object? value)
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

        /// <summary>
        /// Assigns a variable using its ancestor's environment.
        /// </summary>
        /// <param name="distance">Number of environments between current environment and ancestor environment.</param>
        /// <param name="name">Variable name.</param>
        /// <param name="value">Variable value.</param>
        public void AssignAt(int distance, Token name, object? value)
        {
            Ancestor(distance).values[name.lexeme] = value;
        }

        /// <summary>
        /// Steps through a specified number of environments until an environment is located.
        /// </summary>
        /// <param name="distance">Number of environments between current environment and ancestor environment.</param>
        /// <returns>Ancestor environment.</returns>
        private Environment Ancestor(int distance)
        {
            Environment environment = this;
            for (int i = 0; i < distance; i++)
            {
                if (environment.enclosing == null) break; // Should never occur since the ancestor must be non-global
                environment = environment.enclosing;
            }

            return environment;
        }
    }
}
