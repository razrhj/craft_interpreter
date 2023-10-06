#include "AstPrinter.hpp"
#include "GenerateAst.hpp"
#include "Lox.hpp"
#include "Scanner.hpp"
#include "Token.hpp"
#include "TokenType.hpp"
#include <variant>

int main(int argc, char *argv[]) {
  AstPrinter<std::string> ast;
  ast.main(argc, argv);
  return 0;
}
