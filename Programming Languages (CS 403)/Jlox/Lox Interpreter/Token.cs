namespace Lox_Interpreter
{
    class Token
    {
        readonly TokenType type;
        readonly String lexeme;
        readonly Object? literal;
        readonly int line;

        public Token(TokenType type, String lexeme, Object? literal, int line)
        {
            this.type = type;
            this.lexeme = lexeme;
            this.literal = literal;
            this.line = line;
        }

        public override String ToString()
        {
            return "Type is: " + type + " Lexeme is: " + lexeme + " Literal is: " + literal;
            // return type + " " + lexeme + " " + literal;
        }
    }
}
