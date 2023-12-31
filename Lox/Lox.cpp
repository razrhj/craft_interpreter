#include "AstPrinter.hpp"
#include "Expr.hpp"
#include "Parser.hpp"
#include <memory>

bool Lox::hadError = false;

void Lox::run(int argc, char *argvs[]) {
  if (argc > 2) {
    fprintf(stdout, "Usage: jlox [script]\n");
    exit(64);
  } else if (argc == 2) {
    runFile(argvs[1]);
  } else {
    runPrompt();
  }
}

void Lox::runFile(const char *path) {
  std::ifstream ifp(path, std::ios::in | std::ios::binary);
  if (!ifp.is_open()) {
    std::cerr << "open the file failed" << std::endl;
    exit(1);
  }
  std::string source;
  while (std::getline(ifp, source)) {
    run(source);
  }
  ifp.close();

  if (hadError) {
    exit(65);
  }
}

void Lox::runPrompt() {
  for (;;) {
    fprintf(stdout, "> ");
    std::string line;
    if (!std::getline(std::cin, line)) {
      break;
    }
    run(line);
    hadError = false;
  }
}

void Lox::run(const std::string source) {
  Scanner scanner(source);
  std::vector<Token> tokens = scanner.scanTokens();

  std::shared_ptr<Parser> parser = std::make_shared<Parser>(tokens);

  std::shared_ptr<Expr<std::string>> expression = parser->parse<std::string>();

  if (hadError)
    return;

  std::cout << std::make_shared<AstPrinter<std::string>>()->print(expression);

  // for (Token &token : tokens) {
  //   std::cout << token << std::endl;
  // }
}

void Lox::error(const int line, const std::string message) {
  report(line, "", message);
}

void Lox::error(const Token &token, const std::string &message) {
  if (token.getType() == TokenType::eof) {
    report(token.getLine(), " at end", message);
  } else {
    report(token.getLine(), " at '" + token.getLexeme() + "'", message);
  }
}

void Lox::report(const int line, const std::string where,
                 const std::string message) {
  std::cout << "[line " + std::to_string(line) + "] Error" + where + ": " +
                   message
            << std::endl;
  hadError = true;
}
