#include "AstPrinter.hpp"
#include "GenerateAst.hpp"
#include "Lox.hpp"
#include "Scanner.hpp"
#include "Token.hpp"
#include "TokenType.hpp"
#include <variant>

int main(int argc, char *argv[]) {
  Lox lox;
  lox.run(argc, argv);
  return 0;
}
