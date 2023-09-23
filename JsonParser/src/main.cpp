#include "../include/DataTypes.hpp"
#include "../include/json_parser.hpp"
#include <cstdio>
#include <memory>
#include <regex>
#include <type_traits>
#include <unordered_map>

int main(int argc, char *argv[]) {

  std::variant<int, std::string> var;

  var = "123";

  auto &r = std::get<std::string>(var);

  r = "abc";

  printf("%s\n", std::get<std::string>(var).c_str());

  JsonParser jp;

  if (jp.parse("../json_examples/json_example3.json") != nullptr) {
    printf("true\n");
  } else {
    printf("false\n");
  }

  DataTypes::Value val = jp.getValue();

  // auto obj =
  //     std::get<DataTypes::Object>(val[R"("web-app")"][R"("taglib")"]._val);
  // for (auto it = obj._key_val.begin(); it != obj._key_val.end(); it++) {
  //   printf("%-20s\t%-20s\n", it->first.c_str(),
  //          DataTypes::typeToString(it->second->_type_id).c_str());
  // }

  printf("%s\n", val[0].getStringValue().c_str());
  printf("%s\n", val[1].getStringValue().c_str());
  printf("%s\n", val[2].getStringValue().c_str());
  printf("%s\n", val[3]["web-app"]["servlet"][1]["init-param"]
                    ["mailHost"]
                        .getStringValue()
                        .c_str());
  val[3]["web-app"]["servlet"][1]["init-param"]
     ["mailHost"] = 12.453;
  printf("\n%f\n", val[3]["web-app"]["servlet"][1]["init-param"]
                    ["mailHost"]
                        .getDoubleValue()
         );

  jp.write("../json_examples/json_example4.json");
  // printf("%d\n", val[R"("web-app")"][R"("servlet")"]._type_id);
  // auto arr =
  //     std::get<DataTypes::Array>(val[R"("web-app")"][R"("servlet")"]._val);
  // printf("size: %ld\n", arr._vals.size());
  //
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
