#include "./Scanner.hpp"
#include "./parser.hpp"
#include <cstdio>
#include <regex>

int main(int argc, char *argv[]) {
  Scanner sc;
  sc.readFromFile("./json_examples.json");
  sc.scanBuffer();

  for (auto s : Datas::tokens) {
    printf("%s ", s.c_str());
  }

  Parser pr;
  pr.parse();

  return 0;
}
