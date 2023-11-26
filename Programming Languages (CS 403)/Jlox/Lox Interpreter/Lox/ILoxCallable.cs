using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lox_Interpreter.Lox
{
    /// <summary>
    /// Represents the ability to use the implemnting object as a Lox function.
    /// </summary>
    internal interface ILoxCallable
    {
        /// <summary>
        /// Gets the arity of a function and returns it.
        /// </summary>
        /// <returns>The number of the function's paraneters/arguments.</returns>
        public int Arity();

        /// <summary>
        /// Declares the argument of the function in it's own environment and executes it's body.
        /// </summary>
        /// <param name="interpreter">Interpreter used to interpret and execute the body.</param>
        /// <param name="arguments">Arguments of the function</param>
        /// <returns>A value if the "return" keyword is used, otherwise; returns <see langword="null"/>.</returns>
        public Object? Call(Interpreter interpreter, List<Object?> arguments);
    }
}
