#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

// Represents a single local variable.
typedef struct {
    Token name;
    int depth;
} Local;

typedef enum {
    FUNCTION_TYPE,
    SCRIPT_TYPE
} FunctionType;

// Represents the state of the compiler.
typedef struct Compiler {
    struct Compiler* enclosing;
    // For top-level functions.
    ObjFunction* function;
    FunctionType type;
    Local locals[UINT8_COUNT];
    int localSize;
    int scopeDepth;
} Compiler;

// Global variables used throughout the compiler
Parser parser;
Compiler* current = NULL;
Chunk* compilingChunk;

// This function is not finished, but will be used to return the current chunk
// Current could mean many different things.
static Chunk* currentChunk() {
    return &current->function->chunk;
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

// Appends a loop structure to the opcode
static void appendLoop(int loopStart) {
    appendByte(LOOP_OP);

    int offset = currentChunk()->size - loopStart + 2;
    if (offset > UINT16_MAX) error("Loop body too large."); // Reports an error if the loop body is larger than 65,535

    appendByte((offset >> 8) & 0xff);
    appendByte(offset & 0xff);
}

// Makes placeholders for a jump instruction to be patched later and returns a pointer to the start of the placeholders.
static int appendJump(uint8_t instruction) {
    appendByte(instruction);
    appendByte(0xff);
    appendByte(0xff);
    return currentChunk()->size - 2;
}

// Adds a return operator to the end of the chunk's byte.
static void causeReturn() {
    appendByte(NIL_OP);
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

// Fixes placeholders with actual jump value so we know how far to jump.
static void patchJump(int offset) {
    // -2 to adjust for the bytecode for the jump offset itself.
    int jump = currentChunk()->size - offset - 2;

    if (jump > UINT16_MAX) { // Jump is way too far
        error("Too much code to jump over.");
    }

    currentChunk()->instructions[offset] = (jump >> 8) & 0xff;
    currentChunk()->instructions[offset + 1] = jump & 0xff;
}

// Initializes the state of the compiler, including the local variables and how deep in the scope we are,
// as well as top-level functions.
static void initCompiler(Compiler* compiler, FunctionType type) {
    compiler->enclosing = current;
    compiler->function = NULL;
    compiler->type = type;
    compiler->localSize = 0;
    compiler->scopeDepth = 0;

    compiler->function = initNewFunction();
    current = compiler;

    if (type != SCRIPT_TYPE) {
        current->function->name = copyString(parser.previous.sourcePointer, parser.previous.size);
    }

    // Explicitly claims locals slot 0 for the VM
    Local* local = &current->locals[current->localSize++];
    local->depth = 0;
    local->name.sourcePointer = "";
    local->name.size = 0;
}

// Ends the compiler by causing a return.
static ObjFunction* endCompiler() {
    causeReturn();
    ObjFunction* function = current->function;

    #ifdef DEBUG_PRINT_CODE
        if (!parser.hadError) {
            disassembleChunk(currentChunk(), function->name != NULL ? function->name->string : "<script>");
        }
    #endif

    current = current->enclosing; // returns back to parent
    return function;
}

// Starts a new scope by increasing the scope's depth.
static void startScope() {
    current->scopeDepth++;
}

// Ends a scope by decreasing the scope's depth.
static void endScope() {
    current->scopeDepth--;

    // Discards old scopes from previous scopes.
    while (current->localSize > 0 && current->locals[current->localSize - 1].depth > current->scopeDepth) {
        appendByte(POP_OP);
        current->localSize--;
    }
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

// Checks if two identifiers are equal.
// If they are equal, returns true; otherwise, false.
static bool identifiersEqual(Token* a, Token* b) {
    if (a->size != b->size) return false;
    return memcmp(a->sourcePointer, b->sourcePointer, a->size) == 0;
}

// Resolves which local variable in the variable list that an identifier refers to.
// Returns the index of the local variable if found; otherwise, returns -1 to indicate the global scope.
static int resolveLocal(Compiler* compiler, Token* name) {
    for (int i = compiler->localSize - 1; i >= 0; i--) {
        Local* local = &compiler->locals[i];

        if (identifiersEqual(name, &local->name)) {
            if (local->depth == -1) { // Reports an error if the user attempts use itself in its own identifyer.
                error("Can't read local variable in its own initializer.");
            }
            return i;
        }
    }

    return -1;
}

// Adds a local variable to the compiler's current list of local variables
static void addLocal(Token name) {
    // If there are too many variables, it reports an error.
    if (current->localSize == UINT8_COUNT) {
        error("Too many local variables in function.");
        return;
    }
    
    Local* local = &current->locals[current->localSize++];
    local->name = name;
    local->depth = -1; // indicates an uninitialized state
}

// Declares a variable by adding it to it's local scope.
static void declareVariable() {
    // returns if we're in the global scope.
    if (current->scopeDepth == 0) return;

    Token* name = &parser.previous;

    // Current scope is always at the end of the array, so we keep going backwards in the array to find varibles
    // in a higher scope.
    for (int i = current->localSize - 1; i >= 0; i--) {
        Local* local = &current->locals[i];
        if (local->depth != -1 && local->depth < current->scopeDepth) {
            break; 
        }

        // Prevents the user from redefing the same variable in the same scope.
        if (identifiersEqual(name, &local->name)) {
            error("Already a variable with this name in this scope.");
        }
    }
    addLocal(*name);
}

// Parses a variable, consumes an identifier then adds it to the constant table as a string. 
static uint8_t parseVariable(const char* errorMessage) {
    consume(TOKEN_IDENTIFIER, errorMessage);

    declareVariable();
    if (current->scopeDepth > 0) return 0;

    return identifierConstant(&parser.previous);
}

// Declares the variable in the local scope
static void markInitialized() {
    if (current->scopeDepth == 0) return; // global scope does not need this
    current->locals[current->localSize - 1].depth = current->scopeDepth;
}

// Outputs bytecode instruction that defines the variable as global.
static void defineVariable(uint8_t global) {
    // returns if we are in a local scope
    if (current->scopeDepth > 0) {
        markInitialized(); // Marks a variable as initialized after it's initializer has been compiled
        return;
    } 
    appendBytes(DEFINE_GLOBAL_OP, global);
}

// Parses a function's argument list into bytecode
static uint8_t argumentList() {
    uint8_t argCount = 0;
    if (!check(TOKEN_RIGHT_PAREN)) {
        do {
            expression();
            if (argCount == 255) {
                error("Can't have more than 255 arguments."); // Reports an error for too many arguments.
            }
            argCount++;
        } while (match(TOKEN_COMMA)); // Goes through the argument list
    }
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after arguments."); // Reports an error if the argument list is not closed by a ')'
    return argCount;
}

// Parses the and operator into bytecode. It has it's own precedence level.
static void and_(bool canAssign) {
    int endJump = appendJump(JUMP_IF_FALSE_OP);

    appendByte(POP_OP);
    parsePrecedence(AND_PREC);

    patchJump(endJump);
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

// Parses function calls into bytecode
static void call(bool canAssign) {
    uint8_t argCount = argumentList();
    appendBytes(CALL_OP, argCount);
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

// Parses an or operator into bytecode
static void or_(bool canAssign) {
    int elseJump = appendJump(JUMP_IF_FALSE_OP);
    int endJump = appendJump(JUMP_OP);

    patchJump(elseJump);
    appendByte(POP_OP);

    parsePrecedence(OR_PREC);
    patchJump(endJump);
}

// Parses a string
static void string(bool canAssign) {
    appendConstant(OBJ_V(copyString(parser.previous.sourcePointer + 1, parser.previous.size - 2)));
}

// Parses a namedVariable to it's value whenever it is accessed.
static void namedVariable(Token name, bool canAssign) {
    uint8_t getOp, setOp;
    int arg = resolveLocal(current, &name);
    if (arg != -1) { // if within local scope
        getOp = GET_LOCAL_OP;
        setOp = SET_LOCAL_OP;
    } else { // otherwise, we are in the global scope
        arg = identifierConstant(&name);
        getOp = GET_GLOBAL_OP;
        setOp = SET_GLOBAL_OP;
    }
    
    if (canAssign && match(TOKEN_EQUAL)) {
        expression();
        appendBytes(setOp, (uint8_t)arg);
    } else {
        appendBytes(getOp, (uint8_t)arg);
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
    [TOKEN_LEFT_PAREN]    = {grouping, call,   CALL_PREC},
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
    [TOKEN_AND]           = {NULL,     and_,   AND_PREC},
    [TOKEN_CLASS]         = {NULL,     NULL,   NONE_PREC},
    [TOKEN_ELSE]          = {NULL,     NULL,   NONE_PREC},
    [TOKEN_FALSE]         = {literal,  NULL,   NONE_PREC},
    [TOKEN_FOR]           = {NULL,     NULL,   NONE_PREC},
    [TOKEN_FUN]           = {NULL,     NULL,   NONE_PREC},
    [TOKEN_IF]            = {NULL,     NULL,   NONE_PREC},
    [TOKEN_NIL]           = {literal,  NULL,   NONE_PREC},
    [TOKEN_OR]            = {NULL,     or_,    OR_PREC},
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

// Parses a block statement into bytecode
static void block() {
    // Goes through the block statement by statement while still within the block and while not at EOF.
    while (!check(TOKEN_RIGHT_BRACE) && !check(TOKEN_EOF)) {
        declaration();
    }

    consume(TOKEN_RIGHT_BRACE, "Expect '}' after block."); // Reports an error if a block is missing its closing curly bracket.
}

// Parses a function into bytecode
static void function(FunctionType type) {
    Compiler compiler;
    initCompiler(&compiler, type);
    startScope(); 

    consume(TOKEN_LEFT_PAREN, "Expect '(' after function name.");
    // Handles function arguments
    if (!check(TOKEN_RIGHT_PAREN)) {
        do {
            current->function->arity++;
            if (current->function->arity > 255) {
                errorAtCurrent("Can't have more than 255 parameters."); // Reports an error for too many functions
            }
            uint8_t constant = parseVariable("Expect parameter name.");
            defineVariable(constant);
        } while (match(TOKEN_COMMA));
    }
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after parameters.");
    consume(TOKEN_LEFT_BRACE, "Expect '{' before function body.");
    block();

    ObjFunction* function = endCompiler();
    appendBytes(CONSTANT_OP, makeConstant(OBJ_V(function)));
}

// Parses a function declaration via a helper function called function into bytecode
static void funDeclaration() {
    uint8_t global = parseVariable("Expect function name.");
    markInitialized();
    function(FUNCTION_TYPE);
    defineVariable(global);
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

// Parses an expression statement to bytecode.
static void expressionStatement() {
    expression();
    consume(TOKEN_SEMICOLON, "Expect ';' after expression.");
    appendByte(POP_OP);
}

// Parses a for loop statement into bytecode.
static void forStatement() {
    startScope();
    // Reports errors if syntax is missing.
    consume(TOKEN_LEFT_PAREN, "Expect '(' after 'for'.");
    
    // Handles the declaration portion of the for loop
    if (match(TOKEN_SEMICOLON)) {
        // No initializer.
    } 
    else if (match(TOKEN_VAR)) {
        varDeclaration();
    } 
    else {
        expressionStatement();
    }

    int loopStart = currentChunk()->size;
    int exitJump = -1;
    // handles the end point portion of the for loop
    if (!match(TOKEN_SEMICOLON)) {
        expression();
        consume(TOKEN_SEMICOLON, "Expect ';' after loop condition."); // reports an error if a semicolon is missing.

        // Jump out of the loop if the condition is false.
        exitJump = appendJump(JUMP_IF_FALSE_OP);
        appendByte(POP_OP); // Condition.
    }

    // handles the increment portion of the for loop
    if (!match(TOKEN_RIGHT_PAREN)) {
        int bodyJump = appendJump(JUMP_OP);
        int incrementStart = currentChunk()->size;
        expression();
        appendByte(POP_OP);
        consume(TOKEN_RIGHT_PAREN, "Expect ')' after for clauses."); // reports an error if the closing parenthesis is missing.

        appendLoop(loopStart);
        loopStart = incrementStart;
        patchJump(bodyJump);
    }

    statement();
    appendLoop(loopStart);

    if (exitJump != -1) {
        patchJump(exitJump);
        appendByte(POP_OP); // Condition.
    }
    endScope();
}

// Parses a print statement to bytecode.
static void printStatement() {
    expression();
    consume(TOKEN_SEMICOLON, "Expect ';' after value.");
    appendByte(PRINT_OP);
}

// Parses a return statement to bytecode.
static void returnStatement() {
    // Reports an error when trying to return from top-level
    if (current->type == SCRIPT_TYPE) {
        error("Can't return from top-level code.");
    }

    if (match(TOKEN_SEMICOLON)) { // case where return is not specified.
        causeReturn();
    } else { // return is explicitly specified.
        expression();
        consume(TOKEN_SEMICOLON, "Expect ';' after return value."); // Reports an error if statement is lacking ';' at end.
        appendByte(RETURN_OP);
    }
}

// Parses tokens for a while statement into bytecode
static void whileStatement() {
    int loopStart = currentChunk()->size;
    consume(TOKEN_LEFT_PAREN, "Expect '(' after 'while'.");
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after condition.");

    int exitJump = appendJump(JUMP_IF_FALSE_OP);
    appendByte(POP_OP);
    statement();

    appendLoop(loopStart);

    patchJump(exitJump);
    appendByte(POP_OP);
}

static void ifStatement() {
    consume(TOKEN_LEFT_PAREN, "Expect '(' after 'if'.");
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after condition."); 

    int thenJump = appendJump(JUMP_IF_FALSE_OP);
    appendByte(POP_OP);
    statement();

    int elseJump = appendJump(JUMP_OP);

    patchJump(thenJump);
    appendByte(POP_OP);

    // If an else statement is declared, we parse it's statements
    if (match(TOKEN_ELSE)) statement();
    patchJump(elseJump);
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
    if (match(TOKEN_FUN)) { // We've matched a function declaration
        funDeclaration();
    } else if (match(TOKEN_VAR)) { // We've matched a variable declaration
        varDeclaration();
    } else { // Regular statement.
        statement();
    }
    if (parser.inPanicMode) synchronize();
}

// Parses all statements to bytecode.
static void statement() {
    if (match(TOKEN_PRINT)) { // We've reached a print statement
        printStatement();
    }
    else if (match(TOKEN_FOR)) { // We've reached a for statement.
        forStatement();
    }
    else if (match(TOKEN_IF)) { // We've reached an if statement.
        ifStatement();
    }
    else if (match(TOKEN_RETURN)) { // We've reached a return statement.
        returnStatement();
    }
    else if (match(TOKEN_WHILE)) { // We've reached a while statement.
        whileStatement();
    }
    else if (match(TOKEN_LEFT_BRACE)) { // We've reached a block statement.
        startScope();
        block();
        endScope();
    }
    else { // We've reached an expression statement.
        expressionStatement();
    }
}

// Entry point for compiling tokens into bytecode
ObjFunction* compile(const char* sourceCode) {
    initScanner(sourceCode); // initiallizing scanner module with the source Code and getting tokens in return.
    Compiler compiler;

    initCompiler(&compiler, SCRIPT_TYPE);// Initializing the good state of the compiler as a script type

    // Initializing the Parser in a good state.
    parser.hadError = false;
    parser.inPanicMode = false;

    advance(); // Starts the parsing step
    // Entry point for parsing statements and expressions.
    while (!match(TOKEN_EOF)) {
        declaration();
    }

    ObjFunction* function = endCompiler();
    return parser.hadError ? NULL : function;
}