#include "./json_parser.hpp"
#include "DataTypes.hpp"
#include <cstdio>
#include <regex>
#include <unordered_map>

int main(int argc, char *argv[]) {

  JsonParser jp;

  jp.readFromFile("./json_examples.json");
  jp.scanBuffer();
  if (jp.parse() != nullptr) {
    printf("true\n");
  } else {
    printf("false\n");
  }

  // jp.printValues();

  std::unordered_map<std::string, DataTypes::Value *> hash = jp[R"("name")"];

  printf("\"name\": \n");
  for (auto it = hash.begin(); it != hash.end(); it++) {
    printf("\t%s\n", it->first.c_str());
  }

  return 0;
}
