#ifndef JSON_PARSER
#define JSON_PARSER

#include "./DataTypes.hpp"

#include <cstdio>
#include <fstream>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

class JsonParser {
private:
  std::ifstream _ifs;
  std::string _buffer;
  int _idx;
  int _id_arr;
  int _id_obj;
  std::vector<std::string> _tokens;
  std::unordered_map<std::string,
                     std::unordered_map<std::string, DataTypes::Value *>>
      _values;

public:
  JsonParser() {
    _idx = -1;
    _id_arr = _id_obj = 0;
  }
  JsonParser(JsonParser &&) = default;
  JsonParser(const JsonParser &) = delete;
  JsonParser &operator=(JsonParser &&) = default;
  JsonParser &operator=(const JsonParser &) = delete;
  // ~JsonParser() = default;
  ~JsonParser() {
    std::unordered_map<std::string, DataTypes::Value *> empty;
    for (auto it1 = _values.begin(); it1 != _values.end(); it1++) {
      for (auto it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
        delete it2->second;
      }
    }
  }

  std::unordered_map<std::string, DataTypes::Value *> const
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

  // Scan
  void readFromFile(std::string file_path);

  bool isBlank(char c);

  bool isSeparator(char c);

  void scanBuffer();

  // Parse
  std::string Peek();

  void idxForward();

  DataTypes::Value *readNumber(const std::string path);

  DataTypes::Value *readSpecialLiteral(const std::string path);

  DataTypes::Value *readString(const std::string path);

  DataTypes::Value *readKey(const std::string path);

  DataTypes::Value *readValue(const std::string path);

  DataTypes::Value *readObject(const std::string path);

  DataTypes::Value *readArray(const std::string path);

  DataTypes::Value *parse();
};

#endif // !JSON_PARSER
