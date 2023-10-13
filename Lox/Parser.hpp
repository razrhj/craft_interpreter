#pragma once

#include "Expr.hpp"
#include "Token.hpp"
#include <memory>
#include <stdexcept>
#include <vector>

class Parser {
  friend class Lox;

public:
  Parser(const std::vector<Token> &tokens) : tokens(tokens) {}
  Parser() = default;
  Parser(Parser &&) = default;
  Parser(const Parser &) = default;
  Parser &operator=(Parser &&) = delete;
  Parser &operator=(const Parser &) = delete;
  ~Parser() = default;

  template <typename R> std::shared_ptr<Expr<R>> parse() {
    try {
      return expression<R>();
    } catch (const ParserError &error) {
      // return null;
      return nullptr;
    }
  }

private:
  const std::vector<Token> tokens;
  int current = 0;

  template <typename... TokenType> bool match(const TokenType &...types) {
    if ((... || check(types))) {
      advance();
      return true;
    }
    return false;
  }

  bool check(const TokenType &type) {
    if (isAtEnd())
      return false;
    return peek().getType() == type;
  }

  Token advance() {
    if (!isAtEnd()) {
      current++;
    }
    return previous();
  }

  bool isAtEnd() { return peek().getType() == TokenType::eof; }

  Token peek() { return tokens[current]; }

  Token previous() { return tokens[current - 1]; }

  template <typename R> std::shared_ptr<Expr<R>> expression() {
    return equality<R>();
  }

  template <typename R> std::shared_ptr<Expr<R>> equality() {
    std::shared_ptr<Expr<R>> expr = comparison<R>();

    while (match<TokenType>(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL)) {
      Token oprtor = previous();
      std::shared_ptr<Expr<R>> right = comparison<R>();
      expr = std::make_shared<Binary<R>>(expr, oprtor, right);
    }

    return expr;
  }

  template <typename R> std::shared_ptr<Expr<R>> comparison() {
    std::shared_ptr<Expr<R>> expr = term<R>();

    while (match<TokenType>(TokenType::GREATER, TokenType::GREATER_EQUAL,
                            TokenType::LESS, TokenType::LESS_EQUAL)) {
      Token oprtor = previous();
      std::shared_ptr<Expr<R>> right = term<R>();
      expr = std::make_shared<Binary<R>>(expr, oprtor, right);
    }

    return expr;
  }

  template <typename R> std::shared_ptr<Expr<R>> term() {
    std::shared_ptr<Expr<R>> expr = factor<R>();

    while (match<TokenType>(TokenType::MINUS, TokenType::PLUS)) {
      Token oprtor = previous();
      std::shared_ptr<Expr<R>> right = factor<R>();
      expr = std::make_shared<Binary<R>>(expr, oprtor, right);
    }

    return expr;
  }

  template <typename R> std::shared_ptr<Expr<R>> factor() {
    std::shared_ptr<Expr<R>> expr = unary<R>();

    while (match<TokenType>(TokenType::SLASH, TokenType::STAR)) {
      Token oprtor = previous();
      std::shared_ptr<Expr<R>> right = unary<R>();
      expr = std::make_shared<Binary<R>>(expr, oprtor, right);
    }

    return expr;
  }

  template <typename R> std::shared_ptr<Expr<R>> unary() {
    if (match<TokenType>(TokenType::BANG, TokenType::MINUS)) {
      Token oprtor = previous();
      std::shared_ptr<Expr<R>> right = unary<R>();
      return std::make_shared<Unary<R>>(oprtor, right);
    }

    return primary<R>();
  }

  template <typename R> std::shared_ptr<Expr<R>> primary() {
    if (match<TokenType>(TokenType::FALSE)) {
      return std::make_shared<Literal<R>>(Object(false));
    }
    if (match<TokenType>(TokenType::TRUE)) {
      return std::make_shared<Literal<R>>(Object(true));
    }
    if (match<TokenType>(TokenType::NIL)) {
      return std::make_shared<Literal<R>>(Object(TokenType::NIL));
    }

    if (match<TokenType>(TokenType::NUMBER, TokenType::STRING)) {
      return std::make_shared<Literal<R>>(previous().getLiteral());
    }

    if (match<TokenType>(TokenType::LEFT_PAREN)) {
      std::shared_ptr<Expr<R>> expr = expression<R>();
      consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
      return std::make_shared<Grouping<R>>(expr);
    }
    // return std::make_shared<Binary<R>>();
    throw error(peek(), "Expected expression.");
  }

  Token consume(const TokenType &type, const std::string &message) {
    if (check(type))
      return advance();

    throw error(peek(), message);
  }

  class ParserError : public std::runtime_error {
  public:
    ParserError(const std::string &error_message)
        : std::runtime_error(error_message) {}
  };
  std::shared_ptr<ParserError> error(const Token &token,
                                     const std::string &message) {
    Lox::error(token, message);
    return std::make_shared<ParserError>(message);
  }

  void synchronize() {
    advance();

    while (!isAtEnd()) {
      if (previous().getType() == TokenType::SEMICOLON) {
        return;
      }

      switch (peek().getType()) {
      case TokenType::CLASS:
      case TokenType::FUN:
      case TokenType::VAR:
      case TokenType::FOR:
      case TokenType::IF:
      case TokenType::WHILE:
      case TokenType::PRINT:
      case TokenType::RETURN:
        return;
      default:
        break;
      }
    }
    advance();
  }
};
