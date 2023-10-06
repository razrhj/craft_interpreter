#include "Scanner.hpp"

const std::unordered_map<std::string, TokenType> Scanner::keywords = {
    {"and", TokenType::AND},
    {"class", TokenType::CLASS},
    {"else", TokenType::ELSE},
    {"false", TokenType::FALSE},
    {"for", TokenType::FOR},
    {"fun", TokenType::FUN},
    {"if", TokenType::IF},
    {"nil", TokenType::NIL},
    {"or", TokenType::OR},
    {"print", TokenType::PRINT},
    {"return", TokenType::RETURN},
    {"super", TokenType::SUPER},
    {"this", TokenType::THIS},
    {"true", TokenType::TRUE},
    {"var", TokenType::VAR},
    {"while", TokenType::WHILE}

};

void Scanner::scanToken() {
  char c = advance();

  switch (c) {
  case '(':
    addToken(TokenType::LEFT_PAREN);
    break;
  case ')':
    addToken(TokenType::RIGHT_PAREN);
    break;
  case '{':
    addToken(TokenType::LEFT_BRACE);
    break;
  case '}':
    addToken(TokenType::RIGHT_BRACE);
    break;
  case ',':
    addToken(TokenType::COMMA);
    break;
  case '.':
    addToken(TokenType::DOT);
    break;
  case '-':
    addToken(TokenType::MINUS);
    break;
  case '+':
    addToken(TokenType::PLUS);
    break;
  case ';':
    addToken(TokenType::SEMICOLON);
    break;
  case '*':
    addToken(TokenType::STAR);
    break;
  case '!':
    addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
    break;
  case '=':
    addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
    break;
  case '<':
    addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
    break;
  case '>':
    addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
    break;
  case '/':
    if (match('/')) {
      // A comment goes until the end of the line
      while (peek() != '\n' && !isAtEnd()) {
        advance();
      }
    } else {
      addToken(TokenType::SLASH);
    }
    break;
  case ' ':
  case '\r':
  case '\t':
    break;
  case '\n':
    line++;
    break;
  case '"':
    string();
    break;
  default:
    if (isDigit(c)) {
      number();
    } else if (isAlpha(c)) {
      identifier();
    } else {
      Lox::error(line, "Unexpected character.");
    }
    break;
  }
}

void Scanner::string() {
  while (peek() != '"' && !isAtEnd()) {
    if (peek() == '\n') {
      line++;
    }
    advance();
  }

  if (isAtEnd()) {
    Lox::error(line, "Unterminated string");
    return;
  }

  advance();

  std::string value = source.substr(start + 1, current - start - 2);
  addToken(TokenType::STRING, Object(value));
}

void Scanner::number() {
  while (isDigit(peek())) {
    advance();
  }

  if (peek() == '.' && isDigit(peekNext())) {
    advance();

    while (isDigit(peek())) {
      advance();
    }
  }

  addToken(TokenType::NUMBER,
           Object(std::stod(source.substr(start, current - start))));
}

void Scanner::identifier() {
  while (isAlphaNumeric(peek())) {
    advance();
  }

  std::string text = source.substr(start, current - start);
  TokenType type;
  auto it = keywords.find(text);
  if (it != keywords.end()) {
    type = it->second;
  } else {
    type = TokenType::IDENTIFIER;
  }

  addToken(type);
}
