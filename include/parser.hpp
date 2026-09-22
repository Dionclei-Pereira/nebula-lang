#pragma once

#include <memory>
#include <string>
#include <vector>

#include "lexer.hpp"

namespace nbl {

struct Expr {
  virtual ~Expr() = default;
};

struct NumberExpr : Expr {
  double value;

  NumberExpr(double value) : value(value) {}
};

struct StringExpr : Expr {
  std::string value;

  StringExpr(const std::string& value) : value(value) {}
};

struct VariableExpr : Expr {
  std::string name;

  VariableExpr(const std::string& name) : name(name) {}
};

struct BinaryExpr : Expr {
  std::unique_ptr<Expr> left;
  TokenType op;
  std::unique_ptr<Expr> right;

  BinaryExpr(std::unique_ptr<Expr> left, TokenType op,
             std::unique_ptr<Expr> right)
      : left(std::move(left)), op(op), right(std::move(right)) {}
};

struct Statement {
  virtual ~Statement() = default;
};

struct LetStatement : Statement {
  std::string name;
  std::unique_ptr<Expr> value;

  LetStatement(const std::string& name, std::unique_ptr<Expr> value)
      : name(name), value(std::move(value)) {}
};

struct PrintStatement : Statement {
  std::unique_ptr<Expr> value;

  PrintStatement(std::unique_ptr<Expr> value) : value(std::move(value)) {}
};

class Parser {
 private:
  std::vector<Token> tokens;
  size_t current = 0;

  Token peek();
  Token next();
  bool match(TokenType type);

  std::unique_ptr<Expr> expression();
  std::unique_ptr<Expr> term();
  std::unique_ptr<Expr> factor();
  std::unique_ptr<Expr> primary();

  std::unique_ptr<Statement> statement();

 public:
  Parser(const std::vector<Token>& tokens);

  std::vector<std::unique_ptr<Statement>> parse();
};

}  // namespace nbl