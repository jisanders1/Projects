using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lox_Interpreter
{
    /// <summary>
    /// Represents a runtime error within the Lox Interpreter, inherits from the <see cref="Exception"/> class.
    /// </summary>
    internal class RuntimeError : Exception
    {
        public readonly Token token; // Erroneous token.

        /// <summary>
        /// Constructor, calls <see cref="Exception()"/> as well to initiate error message.
        /// </summary>
        /// <param name="token">Token that caused the runtime error.</param>
        /// <param name="message">Error message to be instantiated.</param>
        public RuntimeError(Token token, string message) : base(message)
        {
            this.token = token;
        }
    }
}
