#pragma once
#include "TokenType.hpp"
#include <iostream>
#include <string>
#include <variant>

using Object = std::variant<TokenType, std::string, double, int>;
inline std::string ObjectToString(const Object &obj) {
  std::string ret;

  if (auto p = std::get_if<std::string>(&obj)) {
    ret = *p;
  } else if (auto p = std::get_if<double>(&obj)) {
    ret = std::to_string(*p);
  }

  return ret;
}
class Token {

public:
  Token(TokenType type, std::string lexeme, Object literal, int line)
      : type(type), lexeme(lexeme), literal(literal), line(line) {}

  Token() = delete;
  Token(Token &&) = default;
  Token(const Token &) = default;
  Token &operator=(Token &&) = delete;
  Token &operator=(const Token &) = delete;
  ~Token() = default;

  std::string toString() {
    return typeToString(type) + " " + lexeme + " " +
           std::get<std::string>(literal);
  }

  const TokenType &getType() const { return type; }
  const std::string &getLexeme() const { return lexeme; }
  const Object &getLiteral() const { return literal; }
  const int &getLine() const { return line; }

private:
  const TokenType type;
  const std::string lexeme;
  const Object literal;
  // const Object literal;
  const int line;
};

inline std::ostream &operator<<(std::ostream &out, const Token &token) {
  out << typeToString(token.getType()) << " " << token.getLexeme() << " ";

  if (auto vp = std::get_if<std::string>(&token.getLiteral())) {
    out << *vp;
  } else if (auto vp = std::get_if<double>(&token.getLiteral())) {
    out << *vp;
  }

  out << " " << token.getLine() << std::endl;

  return out;
}
