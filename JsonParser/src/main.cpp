#include "../include/DataTypes.hpp"
#include "../include/json_parser.hpp"
#include <cstdio>
#include <memory>
#include <regex>
#include <unordered_map>

int main(int argc, char *argv[]) {

  JsonParser jp;

  jp.readFromFile("../json_examples/json_example3.json");
  jp.scanBuffer();
  if (jp.parse() != nullptr) {
    printf("true\n");
  } else {
    printf("false\n");
  }

  // jp.printValues();

  std::string str = R"("betaServer")";

  std::unordered_map<std::string, std::shared_ptr<DataTypes::Value>> hash =
      jp[str];

  printf("%s: \n", str.c_str());
  for (auto it = hash.begin(); it != hash.end(); it++) {
    printf("\t%s\n", it->first.c_str());
  }

  return 0;
}
