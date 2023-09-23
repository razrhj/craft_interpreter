#ifndef JSON_PARSER
#define JSON_PARSER

#include "./DataTypes.hpp"

#include <cstdio>
#include <fstream>
#include <memory>
#include <regex>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class JsonParser {
private:
  std::ifstream _ifs;
  std::ofstream _ofs;
  std::string _file_path;
  std::string _buffer;
  int _idx;
  int _id_arr;
  int _id_obj;
  std::vector<std::string> _tokens;
  std::unordered_map<
      std::string,
      std::unordered_map<std::string, std::shared_ptr<DataTypes::Value>>>
      _values;

  /* std::shared_ptr<DataTypes::Value> _val; */
  DataTypes::Value _val;

  // Scan
  void readFromFile(std::string file_path);

  bool isBlank(char c);

  bool isSeparator(char c);

  void scanBuffer();

  // Parse
  std::string Peek();

  void idxForward();

  std::shared_ptr<DataTypes::Value> readNumber(const std::string path);

  std::shared_ptr<DataTypes::Value> readSpecialLiteral(const std::string path);

  std::shared_ptr<DataTypes::Value> readString(const std::string path,
                                               const DataTypes::Type type);

  std::shared_ptr<DataTypes::Value> readKey(const std::string path);

  std::shared_ptr<DataTypes::Value> readValue(const std::string path);

  std::shared_ptr<DataTypes::Value> readObject(const std::string path);

  std::shared_ptr<DataTypes::Value> readArray(const std::string path);

  // Write
  std::string writeNumber(const double num);

  std::string writeSpecialLiteral(const std::string sl);

  std::string writeString(const std::string str);

  std::string writeKey(const std::string key);

  std::string writeValue(const DataTypes::Value &val);

  std::string writeObject(const DataTypes::Object &obj);

  std::string writeArray(const DataTypes::Array &arr);

  void writeToFile(std::string file_path);

  void writeToFile();

public:
  JsonParser() {
    _idx = -1;
    _id_arr = _id_obj = 0;
  }
  JsonParser(JsonParser &&) = default;
  JsonParser(const JsonParser &) = delete;
  JsonParser &operator=(JsonParser &&) = default;
  JsonParser &operator=(const JsonParser &) = delete;
  ~JsonParser() = default;

  JsonParser(const std::string file_path) : _file_path(file_path) {
    _idx = -1;
    _id_arr = _id_obj = 0;
    if (parse(file_path)) {
      printf("parsed successfully!\n");
    } else {
      printf("parsing failed\n");
    }
  }

  /* std::shared_ptr<DataTypes::Value> getValue() { */
  /*   return _val; */
  /* } */

  DataTypes::Value &getValue() { return _val; }

  std::unordered_map<std::string, std::shared_ptr<DataTypes::Value>> const
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

  std::shared_ptr<DataTypes::Value> parse(const std::string path);

  std::string write();

  std::string write(std::string file_path);

  std::string getType(const std::string str, const std::string path);

  std::string getType(const DataTypes::Type type);
};

#endif // !JSON_PARSER
