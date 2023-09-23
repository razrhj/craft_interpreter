#include "../include/json_parser.hpp"
#include <iostream>

int main(int argc, char *argv[]) {

  JsonParser jp("../json_examples/json_example3.json");

  // if (jp.parse("../json_examples/json_example3.json")) {
  //   printf("parsed successfully!\n");
  // } else {
  //   printf("parsing failed\n");
  // }

  DataTypes::Value val = jp.getValue();

  printf("%s\n", val[0].getStringValue().c_str());
  std::cout << val[0].getStringValue() << std::endl;
  printf("%s\n", val[1].getStringValue().c_str());
  std::cout << val[1].getStringValue() << std::endl;
  printf("%s\n", val[2].getStringValue().c_str());
  std::cout << val[2].getStringValue() << std::endl;
  printf("%s\n", val[3]["web-app"]["servlet"][1]["init-param"]["mailHost"]
                     .getStringValue()
                     .c_str());
  val[3]["web-app"]["servlet"][1]["init-param"]["mailHost"] = 12.453;
  printf("\n%f\n", val[3]["web-app"]["servlet"][1]["init-param"]["mailHost"]
                       .getDoubleValue());

  jp.write("../json_examples/json_example4.json");

  return 0;
}
