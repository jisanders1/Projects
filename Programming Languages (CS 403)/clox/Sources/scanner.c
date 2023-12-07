#include <stdio.h>
#include <string.h>
#include "../Headers/common.h"
#include "../Headers/scanner.h"

typedef struct {
    const char* start;
    const char* current;
    int lineNumber;
} Scanner;

Scanner scanner;

// Initializes a scanner starting at the beginning of a specified source
// Keeps track of what line its on.
void initScanner(const char* source) {
    scanner.start = source;
    scanner.current = source;
    scanner.lineNumber = 1;
}

// Checks if the current char, c, is a letter: returns true if it is a letter or underscore; otherwise, false.
static bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

// Checks if the current char, c, is a digit 0-9: returns true if it is a number; otherwise, false.
static bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

// Returns a boolean value: true if at the end of the source and false otherwise.
static bool isAtEnd() {
  return *scanner.current == '\0';
}

// Advances the current pointer and returns the previous character
static char advance() {
    scanner.current++;
    return scanner.current[-1];
}

// Returns the current character without consuming it
static char peek() {
    return *scanner.current;
}

// Returns the next character without consuming the current character.
static char peekNext() {
    if (isAtEnd()) return '\0';
    return scanner.current[1];
}

// Checks if the current character is the expected character: returns true if they match and advances the pointer,
// otherwise it returns false if they don't match.
static bool match(char expected) {
    if (isAtEnd()) return false;
    if (*scanner.current != expected) return false;
    scanner.current++;
    return true;
}

// Initializes a token with a type, its start, its length, and the number it's on and returns it.
static Token makeToken(TokenType type) {
    Token token;
    token.type = type;
    token.sourcePointer = scanner.start;
    token.size = (int)(scanner.current - scanner.start);
    token.lineNumber = scanner.lineNumber;
    return token;
}

// Initiales a token with an error message so the compiler knows to report an error.
static Token errorToken(const char* message) {
    Token token;
    token.type = TOKEN_ERROR;
    token.sourcePointer = message;
    token.size = (int)strlen(message);
    token.lineNumber = scanner.lineNumber;
    return token;
}

// Advances the current pointer when there is whitespace or while in a comment and stops at the first non-whitespace character
static void skipWhitespace() {
    while (true) {
        char c = peek();

        if (c == ' ' || c == '\r' || c == '\t') {
            advance();
        } 
        else if (c == '\n') {
            scanner.lineNumber++;
            advance();
        }
        else if (c == '/') {
            if (peekNext() == '/') {
                // A comment goes until the end of the line.
                while (peek() != '\n' && !isAtEnd()) advance();
            } else {
                return;
            }
        } else {
            return;
        }
    }
}

// Helper function for identifierType(), checks the rest of the "trie" to verify if the current token is a keyword or an identifier.
static TokenType checkKeyword(int start, int length, const char* rest, TokenType type) {
    if (scanner.current - scanner.start == start + length && memcmp(scanner.start + start, rest, length) == 0) {
        return type;
    }

    return TOKEN_IDENTIFIER;
}

// Checks if the identifier is a keyword with a helper function: if so, the token type for said keyword is returned, 
// otherwise the token type for a regular identifier is returned.
static TokenType identifierType() {
    switch (scanner.start[0]) {
        case 'a': return checkKeyword(1, 2, "nd", TOKEN_AND);
        case 'c': return checkKeyword(1, 4, "lass", TOKEN_CLASS);
        case 'e': return checkKeyword(1, 3, "lse", TOKEN_ELSE);
        case 'f': // for "for", "fun", and "false"
            if (scanner.current - scanner.start > 1) {
                switch (scanner.start[1]) {
                    case 'a': return checkKeyword(2, 3, "lse", TOKEN_FALSE);
                    case 'o': return checkKeyword(2, 1, "r", TOKEN_FOR);
                    case 'u': return checkKeyword(2, 1, "n", TOKEN_FUN);
                }
            }
            break;
        case 'i': return checkKeyword(1, 1, "f", TOKEN_IF);
        case 'n': return checkKeyword(1, 2, "il", TOKEN_NIL);
        case 'o': return checkKeyword(1, 1, "r", TOKEN_OR);
        case 'p': return checkKeyword(1, 4, "rint", TOKEN_PRINT);
        case 'r': return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
        case 's': return checkKeyword(1, 4, "uper", TOKEN_SUPER);
        case 't': // for "true" and "this"
            if (scanner.current - scanner.start > 1) {
                switch (scanner.start[1]) {
                    case 'h': return checkKeyword(2, 2, "is", TOKEN_THIS);
                    case 'r': return checkKeyword(2, 2, "ue", TOKEN_TRUE);
                }
            }
            break;
        case 'v': return checkKeyword(1, 2, "ar", TOKEN_VAR);
        case 'w': return checkKeyword(1, 4, "hile", TOKEN_WHILE);
    }
    return TOKEN_IDENTIFIER;
}

// Scans and makes an identifier token.
static Token identifier() {
    while (isAlpha(peek()) || isDigit(peek())) advance();
    return makeToken(identifierType());
}

// Returns a number token and checks if it is a double or a regular int.
static Token number() {
    while (isDigit(peek())) advance();

    // Look for a fractional part.
    if (peek() == '.' && isDigit(peekNext())) {
        // Consume the ".".
        advance();

        while (isDigit(peek())) advance();
    }

    return makeToken(TOKEN_NUMBER);
}

// Returns a string token or an error token (if the string is unterminated).
static Token string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') scanner.lineNumber++;
        advance();
    }

    if (isAtEnd()) return errorToken("Unterminated string.");

    // The closing quote.
    advance();
    return makeToken(TOKEN_STRING);
}

// Returns a new token from the source code.
Token scanToken() {
    skipWhitespace(); // skip any whitespace or comments before scanning a token.
    scanner.start = scanner.current;

    if (isAtEnd()) return makeToken(TOKEN_EOF); // indicates the end of the source has been reached

    char c = advance();
    if (isAlpha(c)) return identifier();
    if (isDigit(c)) return number(); // Handles numbers
    
    switch (c) {
        // Single-character tokens
        case '(': return makeToken(TOKEN_LEFT_PAREN);
        case ')': return makeToken(TOKEN_RIGHT_PAREN);
        case '{': return makeToken(TOKEN_LEFT_BRACE);
        case '}': return makeToken(TOKEN_RIGHT_BRACE);
        case ';': return makeToken(TOKEN_SEMICOLON);
        case ',': return makeToken(TOKEN_COMMA);
        case '.': return makeToken(TOKEN_DOT);
        case '-': return makeToken(TOKEN_MINUS);
        case '+': return makeToken(TOKEN_PLUS);
        case '/': return makeToken(TOKEN_SLASH);
        case '*': return makeToken(TOKEN_STAR);

        // Double-character tokens
        // if match('=') returns true, the left side of ':' is used, otherwise the right side is used
        case '!': return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=': return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<': return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>': return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);

        // Multi-character tokens
        case '"': return string();
    }

    return errorToken("Unexpected character.");
}