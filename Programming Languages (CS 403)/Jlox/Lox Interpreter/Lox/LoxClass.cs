using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace Lox_Interpreter.Lox
{
    /// <summary>
    /// Represents the runtime representation of a Lox class.
    /// </summary>
    internal class LoxClass : ILoxCallable
    {
        public readonly String name; // Name of class
        private readonly Dictionary<String, LoxFunction> methods; // Class's methods
        private readonly bool isInitializer;

        /// <summary>
        /// Initializes an instance of the <see cref="LoxClass"/> class with the specified name and methods.
        /// </summary>
        /// <param name="name">Name of class.</param>
        /// <param name="methods">Dictionary containing the Class's methods.</param>
        public LoxClass(String name, Dictionary<String, LoxFunction> methods)
        {
            this.name = name;
            this.methods = methods;
        }

        public int Arity()
        {
            LoxFunction? initializer = FindMethod("init");
            if (initializer == null) return 0;
            return initializer.Arity();
        }

        /// <summary>
        /// Makes a new instance of a Lox class.
        /// </summary>
        /// <param name="interpreter">Enclosing interpreter used to interpret the class.</param>
        /// <param name="arguments">Class arguments for constructor.</param>
        /// <returns></returns>
        public Object? Call(Interpreter interpreter, List<Object?> arguments)
        {
            LoxInstance instance = new(this);

            LoxFunction? initializer = FindMethod("init");
            if (initializer != null)
            {
                initializer.Bind(instance).Call(interpreter, arguments);
            }

            return instance;
        }

        /// <summary>
        /// Searches the <see cref="methods"/> dictionary for a specified method.
        /// </summary>
        /// <param name="name">Method to search for.</param>
        /// <returns>A <see cref="LoxFunction"/> if found; otherwise <see langword="null"/>.</returns>
        public LoxFunction? FindMethod(String name)
        {
            if (methods.ContainsKey(name))
            {
                return methods[name];
            }

            return null;
        }

        public override String ToString()
        {
            return name;
        }
    }
}
