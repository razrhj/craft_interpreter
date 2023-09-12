#include "./Scanner.hpp"
#include "DataTypes.hpp"
#include <cstdio>
#include <regex>

int main(int argc, char *argv[]) {
  Scanner sc;
  sc.readFromFile("./json_examples.json");
  sc.scanBuffer();
  for (auto s : Datas::tokens) {
    printf("%s ", s.c_str());
  }
  std::regex string_regex(R"("([^\"\\]|(\\(\\|\"|\/|\b|\r|\n|\f|\t)))*")");

  if (std::regex_match(R"("1\\\"24lkj")", string_regex)) {
    printf("true\n");
  } else {
    printf("false\n");
  }

  return 0;
}
