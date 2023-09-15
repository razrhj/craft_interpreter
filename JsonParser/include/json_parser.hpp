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

  void readFromFile(std::string file_path) {
    _ifs.open(file_path, _ifs.in);

    if (!_ifs.is_open()) {
      printf("failed to open %s!\n", file_path.c_str());
      exit(1);
    }

    std::string line;
    while (std::getline(_ifs, line)) {
      _buffer += line;
    }
    // printf("%s\n", _buffer.c_str());
  }

  bool isBlank(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' ||
           c == '\b' || c == '\f';
  }

  bool isSeparator(char c) {
    return c == '{' || c == '}' || c == '[' || c == ']' || c == ':' || c == ',';
  }

  void scanBuffer() {
    std::string temp;
    int queto = 0;
    bool flag = true;
    for (int i = 0; i < _buffer.size(); ++i) {
      char c = _buffer[i];
      if (queto == 0) {
        if (isBlank(c) || isSeparator(c)) {
          if (!temp.empty()) {
            _tokens.push_back(temp);
            temp.clear();
          }
          if (isSeparator(c)) {
            _tokens.push_back(std::string(1, c));
          }
        } else {
          if (c == '"') {
            queto++;
          }
          temp += c;
        }
      } else if (queto == 1) {
        while (c == '\\') {
          flag = !flag;
          temp += c;
          i++;
          c = _buffer[i];
        }
        if (c == '"' && flag) {
          queto++;
        } else {
          flag = true;
        }
        temp += c;
      }
      if (queto == 2) {
        queto = 0;
      }
    }
  }

  std::string Peek() {
    return _idx + 1 < _tokens.size() ? _tokens[_idx + 1] : "";
  }

  void idxForward() { _idx++; }

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
