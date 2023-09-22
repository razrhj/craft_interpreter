#include "../include/DataTypes.hpp"
#include "../include/json_parser.hpp"
#include <cstdio>
#include <memory>
#include <regex>
#include <unordered_map>
#include <type_traits>

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

  auto obj = std::get<DataTypes::Object>(val[R"("web-app")"][R"("taglib")"]._val);
  for (auto it = obj._key_val.begin(); it != obj._key_val.end(); it++) {
    printf("%-20s\t%-20s\n", it->first.c_str(), DataTypes::typeToString(it->second->_type_id).c_str());
  }

  printf("%s\n", val[R"("web-app")"][R"("servlet")"][1][R"("init-param")"][R"("mailHost")"].getStringValue().c_str());
  printf("%d\n", val[R"("web-app")"][R"("servlet")"]._type_id);
  auto arr = std::get<DataTypes::Array>(val[R"("web-app")"][R"("servlet")"]._val);
  printf("size: %d\n", arr._vals.size());

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
