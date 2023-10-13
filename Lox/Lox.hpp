#pragma once

#include "Scanner.hpp"
#include "Token.hpp"
#include "TokenType.hpp"
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <ostream>
#include <ratio>
#include <string>
#include <variant>
#include <vector>

class Lox {
  friend class Scanner;
  friend class Parser;

public:
  Lox() = default;
  Lox(Lox &&) = default;
  Lox(const Lox &) = default;
  Lox &operator=(Lox &&) = default;
  Lox &operator=(const Lox &) = default;
  ~Lox() = default;

  void run(int argc, char *argvs[]);

private:
  static bool hadError;

  static void runFile(const char *path);

  static void runPrompt();

  static void run(const std::string source);

  static void error(const int line, const std::string message);

  static void error(const Token &token, const std::string &message);

  static void report(const int line, const std::string where,
                     const std::string message);
};
