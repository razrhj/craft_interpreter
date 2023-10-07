#include "Lox.hpp"

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

  for (Token &token : tokens) {
    std::cout << token << std::endl;
  }
}

void Lox::error(const int line, const std::string message) {
  report(line, "", message);
}

void Lox::report(const int line, const std::string where,
                 const std::string message) {
  std::cout << "[line " + std::to_string(line) + "] Error" + where + ": " +
                   message
            << std::endl;
  hadError = true;
}
