#include "lexer.hpp"

#include <stdexcept>

Lexer::Lexer(const std::string& code) : code(code) {}

char Lexer::peek() const {
    if (current >= code.size()) return '\0';

    return code[current];
}

char Lexer::next() {
    return code[current++];
}

void Lexer::skipWhiteSpace() {
    while (std::isspace(peek())) {
        next();
    }
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (current < code.size()) {
        skipWhiteSpace();

        if (current >= code.size()) break;

        char c = peek();


        // Integer and Float
        if (std::isdigit(c)) {
            TokenType type = TokenType::Integer;
            std::string number;

            while (std::isdigit(peek()) || peek() == '.') {
                number += next();
                if (peek() == '.') type = TokenType::Float;
            }

            tokens.push_back({
                type,
                number
            });

            continue;
        }

        // String
        if (c == '"') {
            std::string text;
            next();

            while (peek() != '"' && peek() != '\0') {
                text += next();
            }


            tokens.push_back({
                TokenType::String,
                text
            });

            continue;
        }

        if (std::isalpha(c)) {
            std::string word;

            while (std::isalpha(peek())) {
                word += next();
            }

            if (word == "let") {
                tokens.push_back({
                    TokenType::Let,
                    word
                });
            } else if (word == "print") {
                tokens.push_back({
                    TokenType::Print,
                    word
                });
            } else {
                tokens.push_back({
                    TokenType::Identifier,
                    word
                });
            }

            continue;
        }

        switch (c) {
            case '+':
                tokens.push_back({TokenType::Plus, "+"});
                next();
                break;

            case '-':
                tokens.push_back({TokenType::Minus, "-"});
                next();
                break;

            case '*':
                tokens.push_back({TokenType::Star, "*"});
                next();
                break;

            case '/':
                tokens.push_back({TokenType::Slash, "/"});
                next();
                break;

            case '=':
                tokens.push_back({TokenType::Equal, "="});
                next();
                break;

            case '(':
                tokens.push_back({TokenType::LeftParen, "("});
                next();
                break;

            case ')':
                tokens.push_back({TokenType::RightParen, ")"});
                next();
                break;
            default:
                throw std::runtime_error(
                    std::string("unexpected character: ") + c
                );
                break;
        }
    }

    tokens.push_back({TokenType::End, ""});

    return tokens;
}