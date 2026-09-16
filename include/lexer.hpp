#pragma once
#include <string>
#include <vector>

enum class TokenType {
    Integer,
    String,
    Identifier,

    Var,
    Print,

    Plus,
    Minus,
    Star,
    Slash,

    Equal,

    LeftParen,
    RightParen,

    End
};

struct Token {
    TokenType type;
    std::string value;
};

class Lexer {
private:
    std::string code;
    size_t current = 0;
    
    char peek() const;
    char next();

    void skipWhiteSpace();

public:
    Lexer(const std::string& code);

    std::vector<Token> tokenize();
};