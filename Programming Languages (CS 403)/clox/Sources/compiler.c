#include <stdio.h>
#include <stdlib.h>
#include "../Headers/common.h"
#include "../Headers/compiler.h"
#include "../Headers/scanner.h"

#ifdef DEBUG_PRINT_CODE
    #include "../Headers/debug.h"
#endif

// Represents a parser that stores the current and previous tokens
typedef struct {
    Token current;
    Token previous;
    bool hadError;
    bool inPanicMode;
} Parser;

// Represents the precence level for the Lox language.
typedef enum {
    NONE_PREC,
    ASSIGNMENT_PREC,  // =
    OR_PREC,          // or
    AND_PREC,         // and
    EQUALITY_PREC,    // == !=
    COMPARISON_PREC,  // < > <= >=
    TERM_PREC,        // + -
    FACTOR_PREC,      // * /
    UNARY_PREC,       // ! -
    CALL_PREC,        // . ()
    PRIMARY_PREC
} Precedence;

// Simpe typedef for a function type that takes no arguments and returns nothing.
typedef void (*ParseFn)(bool canAssign);

// Represents the rules for parsing and stores it's precedence.
typedef struct {
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
} ParseRule;

Parser parser;
Chunk* compilingChunk;

// This function is not finished, but will be used to return the current chunk
// Current could mean many different things.
static Chunk* currentChunk() {
    return compilingChunk;
}

static void errorAt(Token* token, const char* message) {
    // If the parser is currently in panic mode, 
    // an error has already been tracked and we don't want errors to keep happening 
    // because of that.
    if (parser.inPanicMode) return;

    parser.inPanicMode = true;
    fprintf(stderr, "[line %d] Error", token->lineNumber);

    switch (token->type) {
        case TOKEN_EOF:
            fprintf(stderr, " at end");
            break;
        case TOKEN_ERROR:
            break;
        default:
            fprintf(stderr, " at '%.*s'", token->size, token->sourcePointer);
    }

    fprintf(stderr, ": %s\n", message);
    parser.hadError = true;
}

// Reports an error at the previous token.
static void error(const char* message) {
    errorAt(&parser.previous, message);
}

// Wrapper function for reporting an error.
static void errorAtCurrent(const char* message) {
    errorAt(&parser.current, message);
}

// Advances through tokens via the scan token function.
// Only need to scan a token when the previous one has already been processed.
static void advance() {
    parser.previous = parser.current;

    while (true) {
        parser.current = scanToken();
        if (parser.current.type != TOKEN_ERROR) break; // Breaks if an error token is encountered.

        errorAtCurrent(parser.current.sourcePointer);
    }
}

// Checks if a type is equal to the current parser's type: if so, we advance.
// If it is not equal, we report an error.
static void consume(TokenType type, const char* message) {
    if (parser.current.type == type) {
        advance();
        return;
    }

    errorAtCurrent(message);
}

// Helper function that returns the boolean expression for checking the current type and the type passed in.
static bool check(TokenType type) {
    return type == parser.current.type;
}

// Checks if a tokentype matches the current token.
// If it does match, we advance to the next token and return true.
// If not, we do not advance and return false.
static bool match(TokenType type) {
    if (!check(type)) return false;
    advance();
    return true;
}

// Appends a byte to the end of the current chunk's instructions.
static void appendByte(uint8_t byte) {
    addChunkInstruction(currentChunk(), byte, parser.previous.lineNumber);
}

// Appends two bytes to the end of the current chunk's instructions.
static void appendBytes(uint8_t byte1, uint8_t byte2) {
    appendByte(byte1);
    appendByte(byte2);
}

// Adds a return operator to the end of the chunk's byte.
static void causeReturn() {
    appendByte(RETURN_OP);
}

static uint8_t makeConstant(Value value) {
    int constant = addConstant(currentChunk(), value);

    // Can only store 256 constants in a chunk.
    // Reports an error if there are too many constants.
    if (constant > UINT8_MAX) {
        error("Too many constants in one chunk.");
        return 0;
    }

    return (uint8_t)constant;
}

// Appends a constant op code and a constant to the end of the current chunks function.
static void appendConstant(Value value) {
    appendBytes(CONSTANT_OP, makeConstant(value));
}

// Ends the compiler by causing a return.
static void endCompiler() {
    causeReturn();
    #ifdef DEBUG_PRINT_CODE
        if (!parser.hadError) {
            disassembleChunk(currentChunk(), "code");
        }
    #endif
}

// Following lines are present in order for the expression functions to be able to call the functions,
// even though they are defined later in the code.
static void expression();
static void statement();
static void declaration();
static ParseRule* getRule(TokenType type);
static void parsePrecedence(Precedence precedence);

// Makes a constant given an token
static uint8_t identifierConstant(Token* name) {
    return makeConstant(OBJ_V(copyString(name->sourcePointer, name->size)));
}

// parses a variable, consumes an identifier then adds it to the constant table as a string. 
static uint8_t parseVariable(const char* errorMessage) {
    consume(TOKEN_IDENTIFIER, errorMessage);
    return identifierConstant(&parser.previous);
}

// Outputs bytecode instruction that defines the variable as global.
static void defineVariable(uint8_t global) {
    appendBytes(DEFINE_GLOBAL_OP, global);
}

// Evaluates binary operators
static void binary(bool canAssign) {
    TokenType operatorType = parser.previous.type;
    ParseRule* rule = getRule(operatorType);
    parsePrecedence((Precedence)(rule->precedence + 1));

    switch (operatorType) {
        case TOKEN_BANG_EQUAL:    appendBytes(EQUAL_OP, NOT_OP); break;
        case TOKEN_EQUAL_EQUAL:   appendByte(EQUAL_OP); break;
        case TOKEN_GREATER:       appendByte(GREATER_OP); break;
        case TOKEN_GREATER_EQUAL: appendBytes(LESS_OP, NOT_OP); break;
        case TOKEN_LESS:          appendByte(LESS_OP); break;
        case TOKEN_LESS_EQUAL:    appendBytes(GREATER_OP, NOT_OP); break;
        case TOKEN_PLUS: appendByte(ADD_OP); break;
        case TOKEN_MINUS: appendByte(SUBTRACT_OP); break;
        case TOKEN_STAR: appendByte(MULTIPLY_OP); break;
        case TOKEN_SLASH: appendByte(DIVIDE_OP); break;
        default: return; // Unreachable.
    }
}

// Evaluates literals, currently false, nil, and tru
static void literal(bool canAssign) {
    switch (parser.previous.type) {
        case TOKEN_FALSE: appendByte(FALSE_OP); break;
        case TOKEN_NIL: appendByte(NIL_OP); break;
        case TOKEN_TRUE: appendByte(TRUE_OP); break;
        default: return; // Unreachable.
    }
}

// Handles grouping expressions, contains a recursive call to evaluate the inner expression.
static void grouping(bool canAssign) {
    expression(); // recursive call to evaluate inner expression.
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression."); // throws an error if a right paren is missing.
}

// Converts the sourceCode's string number to an actual number and appends it to the back of the chunk.
static void number(bool canAssign) {
    double value = strtod(parser.previous.sourcePointer, NULL); // string -> double
    appendConstant(NUMBER_V(value));
}

// Parses a string
static void string(bool canAssign) {
    appendConstant(OBJ_V(copyString(parser.previous.sourcePointer + 1, parser.previous.size - 2)));
}

static void namedVariable(Token name, bool canAssign) {
    uint8_t arg = identifierConstant(&name);
    if (canAssign && match(TOKEN_EQUAL)) {
        expression();
        appendBytes(SET_GLOBAL_OP, arg);
    } else {
        appendBytes(GET_GLOBAL_OP, arg);
    }
}

// Parses a named variable
static void variable(bool canAssign) {
    namedVariable(parser.previous, canAssign);
}

// Evaluates a unary expression
static void unary(bool canAssign) {
    TokenType operatorType = parser.previous.type; // Gets the negation symbol

    // Compile the operand.
    parsePrecedence(UNARY_PREC);

    // Emit the operator instruction.
    switch (operatorType) {
        case TOKEN_BANG: appendByte(NOT_OP); break;
        case TOKEN_MINUS: appendByte(NEGATE_OP); break;
        default: return; // Unreachable.
    }
}

// Represents the parsing rules for Lox. Will be filled out as time goes on
ParseRule rules[] = {
    [TOKEN_LEFT_PAREN]    = {grouping, NULL,   NONE_PREC},
    [TOKEN_RIGHT_PAREN]   = {NULL,     NULL,   NONE_PREC},
    [TOKEN_LEFT_BRACE]    = {NULL,     NULL,   NONE_PREC}, 
    [TOKEN_RIGHT_BRACE]   = {NULL,     NULL,   NONE_PREC},
    [TOKEN_COMMA]         = {NULL,     NULL,   NONE_PREC},
    [TOKEN_DOT]           = {NULL,     NULL,   NONE_PREC},
    [TOKEN_MINUS]         = {unary,    binary, TERM_PREC},
    [TOKEN_PLUS]          = {NULL,     binary, TERM_PREC},
    [TOKEN_SEMICOLON]     = {NULL,     NULL,   NONE_PREC},
    [TOKEN_SLASH]         = {NULL,     binary, FACTOR_PREC},
    [TOKEN_STAR]          = {NULL,     binary, FACTOR_PREC},
    [TOKEN_BANG]          = {unary,    NULL,   NONE_PREC},
    [TOKEN_BANG_EQUAL]    = {NULL,     binary, EQUALITY_PREC},
    [TOKEN_EQUAL]         = {NULL,     NULL,   NONE_PREC},
    [TOKEN_EQUAL_EQUAL]   = {NULL,     binary, EQUALITY_PREC},
    [TOKEN_GREATER]       = {NULL,     binary, COMPARISON_PREC},
    [TOKEN_GREATER_EQUAL] = {NULL,     binary, COMPARISON_PREC},
    [TOKEN_LESS]          = {NULL,     binary, COMPARISON_PREC},
    [TOKEN_LESS_EQUAL]    = {NULL,     binary, COMPARISON_PREC},
    [TOKEN_IDENTIFIER]    = {variable, NULL,   NONE_PREC},
    [TOKEN_STRING]        = {string,   NULL,   NONE_PREC},
    [TOKEN_NUMBER]        = {number,   NULL,   NONE_PREC},
    [TOKEN_AND]           = {NULL,     NULL,   NONE_PREC},
    [TOKEN_CLASS]         = {NULL,     NULL,   NONE_PREC},
    [TOKEN_ELSE]          = {NULL,     NULL,   NONE_PREC},
    [TOKEN_FALSE]         = {literal,  NULL,   NONE_PREC},
    [TOKEN_FOR]           = {NULL,     NULL,   NONE_PREC},
    [TOKEN_FUN]           = {NULL,     NULL,   NONE_PREC},
    [TOKEN_IF]            = {NULL,     NULL,   NONE_PREC},
    [TOKEN_NIL]           = {literal,  NULL,   NONE_PREC},
    [TOKEN_OR]            = {NULL,     NULL,   NONE_PREC},
    [TOKEN_PRINT]         = {NULL,     NULL,   NONE_PREC},
    [TOKEN_RETURN]        = {NULL,     NULL,   NONE_PREC},
    [TOKEN_SUPER]         = {NULL,     NULL,   NONE_PREC},
    [TOKEN_THIS]          = {NULL,     NULL,   NONE_PREC},
    [TOKEN_TRUE]          = {literal,  NULL,   NONE_PREC},
    [TOKEN_VAR]           = {NULL,     NULL,   NONE_PREC},
    [TOKEN_WHILE]         = {NULL,     NULL,   NONE_PREC},
    [TOKEN_ERROR]         = {NULL,     NULL,   NONE_PREC},
    [TOKEN_EOF]           = {NULL,     NULL,   NONE_PREC},
};

static void parsePrecedence(Precedence precedence) {
    advance();
    ParseFn prefixRule = getRule(parser.previous.type)->prefix;
    if (prefixRule == NULL) {
        error("Expect expression.");
        return;
    }

    bool canAssign = precedence <= ASSIGNMENT_PREC;
    prefixRule(canAssign);

    while (precedence <= getRule(parser.current.type)->precedence) {
        advance();
        ParseFn infixRule = getRule(parser.previous.type)->infix;
        infixRule(canAssign);
    }

    // If the equal sign has not been consumed, then that means we have an error to report.
    if (canAssign && match(TOKEN_EQUAL)) error("Invalid assignment target.");
}

// Returns the parsing rules from the rules table
static ParseRule* getRule(TokenType type) {
    return &rules[type];
}

// Parses all expressions including string and number literals and booleans.
static void expression() {
    parsePrecedence(ASSIGNMENT_PREC);
}

// Parses a variable declaration to bytecode.
static void varDeclaration() {
    uint8_t global = parseVariable("Expect variable name.");

    if (match(TOKEN_EQUAL)) {
        expression();
    } else { //  initializes variables to nil by default
        appendByte(NIL_OP);
    }
    consume(TOKEN_SEMICOLON, "Expect ';' after variable declaration."); // Reports an error if there is no semicolon.

    defineVariable(global);
}

static void expressionStatement() {
    expression();
    consume(TOKEN_SEMICOLON, "Expect ';' after expression.");
    appendByte(POP_OP);
}

// Parses a print statement to bytecode.
static void printStatement() {
    expression();
    consume(TOKEN_SEMICOLON, "Expect ';' after value.");
    appendByte(PRINT_OP);
}

// Gets the parsing back to a good state after an error has occured to prevent the cascading of errors from one error.
// A good state is considered to be the start of the next declaration statement.
static void synchronize() {
    parser.inPanicMode = false;

    while (parser.current.type != TOKEN_EOF) {
        if (parser.previous.type == TOKEN_SEMICOLON) return;
        switch (parser.current.type) {
            case TOKEN_CLASS:
            case TOKEN_FUN:
            case TOKEN_VAR:
            case TOKEN_FOR:
            case TOKEN_IF:
            case TOKEN_WHILE:
            case TOKEN_PRINT:
            case TOKEN_RETURN:
                return;

            default:
                ; // Do nothing.
        }

        advance();
    }
}

// Parses declaration statements to bytecode
static void declaration() {
    if (match(TOKEN_VAR)) {
        varDeclaration();
    } else {
        statement();
    }
    if (parser.inPanicMode) synchronize();
}

// Parses all statements to bytecode.
static void statement() {
    if (match(TOKEN_PRINT)) {
        printStatement();
    } else {
        expressionStatement();
    }
}

// Entry point for compiling tokens into bytecode
bool compile(const char* sourceCode, Chunk* chunk) {
    initScanner(sourceCode);
    compilingChunk = chunk; // initializing the current chunk

    // Initializing the Parser in a good state.
    parser.hadError = false;
    parser.inPanicMode = false;

    advance(); // Starts the parsing step
    // Entry point for parsing statements and expressions.
    while (!match(TOKEN_EOF)) {
        declaration();
    }
    endCompiler();
    return !parser.hadError;
}