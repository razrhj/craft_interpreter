#include "../include/json_parser.hpp"
#include <iostream>

int main(int argc, char *argv[]) {

  // JsonParser jp("../json_examples/json_example3.json");

  JsonParser jp;

  // if (jp.parse("../json_examples/json_example3.json")) {
  //   printf("parsed successfully!\n");
  // } else {
  //   printf("parsing failed\n");
  // }

  jsondatas::Value &val = jp.getValue();

  // printf("%s\n", val[0].getStringValue().c_str());
  // std::cout << val[0].getStringValue() << std::endl;
  // printf("%s\n", val[1].getStringValue().c_str());
  // std::cout << val[1].getStringValue() << std::endl;
  // printf("%s\n", val[2].getStringValue().c_str());
  // std::cout << val[2].getStringValue() << std::endl;
  // printf("%s\n", val[3]["web-app"]["servlet"][1]["init-param"]["mailHost"]
  //                    .getStringValue()
  //                    .c_str());
  // val[3]["web-app"]["servlet"][1]["init-param"]["mailHost"] = 12.453;
  // printf("\n%f\n", val[3]["web-app"]["servlet"][1]["init-param"]["mailHost"]
  //                      .getDoubleValue());
  //
  val.setArray();
  val[0] = true;
  val[1].setArray();
  val[1][0] = "false";
  val[1][1].setObject();
  val[1][1]["name"] = "fengtinghao";
  val[1][1]["gender"] = "male";
  val[1][1]["advantages"].setArray();
  val[1][1]["advantages"][0] = "handsome";
  val[1][1]["advantages"][1] = std::string("hardwork");

  printf("%s\n", val[1][1]["advantages"][1].getStringValue().c_str());

  jp.write("../json_examples/json_example4.json");

  return 0;
}
