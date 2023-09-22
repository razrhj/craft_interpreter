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

  auto val = jp.getValue();

  auto obj = std::get<DataTypes::Object>(val._val);
  for (auto it = obj._key_val.begin(); it != obj._key_val.end(); it++) {
    printf("%s\n", it->first.c_str());
    auto obj1 = std::get<DataTypes::Object>(it->second->_val);
    for (auto it1 = obj1._key_val.begin(); it1 != obj1._key_val.end(); it1++) {
      printf("%s\n", it1->first.c_str());
    }
  }

  printf("%s\n", val[R"("web-app")"][R"("servlet")"][0][R"("servlet-name")"].getStringValue().c_str());

  // jp.printValues();

  std::string str = R"("cofax.tld")";

  std::unordered_map<std::string, std::shared_ptr<DataTypes::Value>> hash =
      jp[str];

  printf("%s: \n", str.c_str());
  for (auto it = hash.begin(); it != hash.end(); it++) {
    printf("\t%s\n\t%s\n", it->first.c_str(),
           jp.getType(it->second->_type_id).c_str());
    // jp.getType(str, it->second->_path).c_str());
  }

  return 0;
}
