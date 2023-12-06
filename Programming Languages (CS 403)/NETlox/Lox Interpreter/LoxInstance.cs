using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lox_Interpreter
{
    /// <summary>
    /// Represents the runtime representation of a Lox class instance.
    /// </summary>
    internal class LoxInstance
    {
        private LoxClass klass; // Name of class instance
        private readonly Dictionary<string, object?> fields = new();

        /// <summary>
        /// Initializes an instance of the <see cref="LoxInstance"/> class with the specified class.
        /// </summary>
        /// <param name="klass">Name of class instance.</param>
        public LoxInstance(LoxClass klass)
        {
            this.klass = klass;
        }

        /// <summary>
        /// Retrieves a fields value if the class instance contains it. Throws an exception if the field is not defined.
        /// </summary>
        /// <param name="name">Field name to be retrieved.</param>
        /// <returns>The value of the field.</returns>
        /// <exception cref="RuntimeError"></exception>
        public object? Get(Token name)
        {
            if (fields.ContainsKey(name.lexeme))
            {
                return fields[name.lexeme];
            }

            LoxFunction? method = klass.FindMethod(name.lexeme);
            if (method != null) return method.Bind(this);

            throw new RuntimeError(name, "Undefined property '" + name.lexeme + "'.");
        }

        /// <summary>
        /// Sets a fields value with the specified value.
        /// </summary>
        /// <param name="name">Field to set.</param>
        /// <param name="value">Value to set field to.</param>
        public void Set(Token name, object? value)
        {
            fields[name.lexeme] = value;
        }

        public override string ToString()
        {
            return klass.name + " instance";
        }
    }
}
