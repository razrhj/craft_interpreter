#ifndef JSON_PARSER
#define JSON_PARSER

#include "./io.hpp"
#include "./jsondatas.hpp"
#include "./scanner.hpp"

#include <cstdio>
#include <memory>
#include <regex>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class JsonParser {
private:
  IO io;
  Scanner sc;

  // std::ifstream _ifs;
  // std::ofstream _ofs;
  std::string _file_path;
  std::string _buffer;

  int _idx;
  int _id_arr;
  int _id_obj;
  std::vector<std::string> _tokens;
  std::unordered_map<
      std::string,
      std::unordered_map<std::string, std::shared_ptr<jsondatas::Value>>>
      _values;

  /* std::shared_ptr<jsondatas::Value> _val; */
  jsondatas::Value _val;

  // Parse
  std::string Peek();

  void idxForward();

  std::shared_ptr<jsondatas::Value> parseNumber(const std::string path);

  std::shared_ptr<jsondatas::Value> parseSpecialLiteral(const std::string path);

  std::shared_ptr<jsondatas::Value> parseString(const std::string path,
                                                const jsondatas::Type type);

  std::shared_ptr<jsondatas::Value> parseKey(const std::string path);

  std::shared_ptr<jsondatas::Value> parseValue(const std::string path);

  std::shared_ptr<jsondatas::Value> parseObject(const std::string path);

  std::shared_ptr<jsondatas::Value> parseArray(const std::string path);

public:
  // JsonParser() {
  //   _idx = -1;
  //   _id_arr = _id_obj = 0;
  // }
  JsonParser() = delete;
  JsonParser(JsonParser &&) = default;
  JsonParser(const JsonParser &) = delete;
  JsonParser &operator=(JsonParser &&) = default;
  JsonParser &operator=(const JsonParser &) = delete;
  ~JsonParser() = default;

  JsonParser(const std::string file_path) : _file_path(file_path) {
    _idx = -1;
    _id_arr = _id_obj = 0;

    if (parse(file_path)) {
      printf("parsed correctly!\n");
    } else {
      printf("parsing failed! Occurred certain Wrong!\n");
    }
  }

  JsonParser(std::istream &in) {
    _idx = -1;
    _id_arr = _id_obj = 0;
    _file_path = "";

    if (parse(in)) {
      printf("parsed correctly!\n");
    } else {
      printf("parsing failed! Occurred certain Wrong!\n");
    }
  }

  /* std::shared_ptr<jsondatas::Value> getValue() { */
  /*   return _val; */
  /* } */

  jsondatas::Value &getValue() { return _val; }

  std::unordered_map<std::string, std::shared_ptr<jsondatas::Value>> const
  operator[](const std::string str) {
    return _values[str];
  }

  void printValues() {
    for (auto it1 = _values.begin(); it1 != _values.end(); ++it1) {
      printf("string:%s\n", it1->first.c_str());
      for (auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
        printf("\t%s\n", it2->first.c_str());
      }
      printf("\n");
    }
  }

  void printTokens() {
    for (auto s : _tokens) {
      printf("%s\n", s.c_str());
    }
  }

  std::shared_ptr<jsondatas::Value> parse(const std::string path);
  std::shared_ptr<jsondatas::Value> parse(std::istream &in);

  std::string write() { return io.write(_val); }

  std::string write(std::string file_path) { return io.write(_val, file_path); }

  std::string getType(const std::string str, const std::string path);

  std::string getType(const jsondatas::Type type);

};

#endif // !JSON_PARSER
