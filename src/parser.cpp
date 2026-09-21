#include "parser.hpp"

#include <stdexcept>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

Token Parser::peek() {
    return tokens[current];
}

Token Parser::next() {
    return tokens[current++];
}

bool Parser::match(TokenType type) {
    if (peek().type == type) {
        next();
        return true;
    }

    return false;
}

std::unique_ptr<Expr> Parser::primary() {
    Token token = next();

    if (token.type == TokenType::Float || token.type == TokenType::Integer) {
        return std::make_unique<NumberExpr>(std::stod(token.value));
    }

    if (token.type == TokenType::String) {
        return std::make_unique<StringExpr>(token.value);
    }

    if (token.type == TokenType::Identifier) {
        return std::make_unique<VariableExpr>(token.value);
    }

    if (token.type == TokenType::LeftParen) {
        std::unique_ptr<Expr> expr = expression();

        if (!match(TokenType::RightParen)) {
            throw std::runtime_error("Expected ')'");
        }

        return expr;
    }

    throw std::runtime_error("Invalid expression");
}

std::unique_ptr<Expr> Parser::factor() {
    auto expr = primary();

    while (peek().type == TokenType::Slash || peek().type == TokenType::Star) {
        Token op = next();
        auto right = primary();

        return std::make_unique<BinaryExpr>(std::move(expr), op.type, std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::term() {
    auto expr = factor();
    while (peek().type == TokenType::Plus || peek().type == TokenType::Minus) {
        auto op = next();
        auto right = factor();
        return std::make_unique<BinaryExpr>(std::move(expr), op.type, std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::expression() {
    return term();
}