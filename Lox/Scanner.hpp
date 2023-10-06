#pragma once

#include "Lox.hpp"
#include "Token.hpp"
#include "TokenType.hpp"
#include <cstddef>
#include <exception>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

class Scanner {
  friend class Lox;

public:
  Scanner(const std::string source) : source(source) {}
  Scanner() = default;
  Scanner(Scanner &&) = default;
  Scanner(const Scanner &) = default;
  Scanner &operator=(Scanner &&) = delete;
  Scanner &operator=(const Scanner &) = delete;
  ~Scanner() = default;

private:
  const std::string source;
  std::vector<Token> tokens;
  static const std::unordered_map<std::string, TokenType> keywords;

  int start = 0;
  int current = 0;
  int line = 1;

  bool isAtEnd() { return current >= source.size(); }

  char advance() { return source[current++]; }

  bool match(char expected) {
    if (isAtEnd())
      return false;
    if (source[current] != expected)
      return false;

    current++;
    return true;
  }

  char peek() {
    if (isAtEnd()) {
      return '\0';
    }
    return source[current];
  }

  char peekNext() {
    if (current + 1 >= source.size())
      return '\0';
    return source[current + 1];
  }

  bool isDigit(const char c) { return c >= '0' && c <= '9'; }

  bool isAlpha(const char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
  }

  bool isAlphaNumeric(const char c) { return isAlpha(c) || isDigit(c); }

  void addToken(const TokenType type) { addToken(type, Object()); }

  void addToken(const TokenType type, const Object literal) {
    std::string text = source.substr(start, current - start);
    tokens.emplace_back(type, text, literal, line);
  }

  std::vector<Token> scanTokens() {
    while (!isAtEnd()) {
      start = current;
      scanToken();
    }

    tokens.emplace_back(TokenType::eof, "", Object(), line);
    return tokens;
  }

  void scanToken();

  void string();

  void number();

  void identifier();
};
