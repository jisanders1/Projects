using System;

namespace Lox_Interpreter
{
    /// <summary>
    /// Represents a token that has a type, lexeme, a literal, and the line it is on.
    /// </summary>
    internal class Token
    {
        public readonly TokenType type; // Type of token
        public readonly string lexeme; // these are the blobs of raw source code
        public readonly object? literal; // can be null, represents acutal values that the code needs/uses
        public readonly int line;

        /// <summary>
        /// Initializes an instace of the <see cref="Token"/> class.
        /// </summary>
        /// <param name="type">Specifies the type of token.</param>
        /// <param name="lexeme">The raw representation of the code from the source.</param>
        /// <param name="literal">Literal value of the token if it is a string or a number.</param>
        /// <param name="line">Line number the token is present on.</param>
        public Token(TokenType type, string lexeme, object? literal, int line)
        {
            this.type = type;
            this.lexeme = lexeme;
            this.literal = literal;
            this.line = line;
        }

        /// <summary>
        /// Generates a string representation of a token.
        /// </summary>
        /// <returns>A string with a token's type, lexeme, and literal value if applicable.</returns>
        public override string ToString()
        {
            return type + " " + lexeme + " " + literal;
        }
    }
}
