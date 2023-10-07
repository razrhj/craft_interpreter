#pragma once

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class GenerateAst {
public:
  GenerateAst() = default;
  GenerateAst(GenerateAst &&) = default;
  GenerateAst(const GenerateAst &) = default;
  GenerateAst &operator=(GenerateAst &&) = default;
  GenerateAst &operator=(const GenerateAst &) = default;
  ~GenerateAst() = default;

  void run(int argc, char *argv[]) {
    if (argc != 2) {
      std::cerr << "Usage: generate_ast <output directory>" << std::endl;
      exit(64);
    }
    std::string outputDir = argv[1];

    defineAst(outputDir, "Expr",
              std::vector<std::string>{
                  "Binary   : Expr left, Token oprtor, Expr right",
                  "Grouping : Expr expression", "Literal  : Object value",
                  "Unary    : Token oprtor, Expr right"});
  }

private:
  static void defineAst(const std::string &outputDir,
                        const std::string &baseName,
                        const std::vector<std::string> &types);

  static void defineVisitor(std::ofstream &ofs, const std::string &baseName,
                            const std::vector<std::string> types);

  static void defineType(std::ofstream &ofs, const std::string baseName,
                         const std::string &className, std::string &fieldList);
};
