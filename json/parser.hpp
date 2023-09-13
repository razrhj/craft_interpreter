#ifndef PARSER
#define PARSER

#include "./DataTypes.hpp"
#include <regex>

class Parser {
public:
  Parser() {
    _idx = -1;
    _id_arr = _id_obj = 0;
  }
  Parser(Parser &&) = default;
  Parser(const Parser &) = default;
  Parser &operator=(Parser &&) = default;
  Parser &operator=(const Parser &) = default;
  ~Parser() = default;

  std::string Peek() {
    return _idx + 1 < Datas::tokens.size() ? Datas::tokens[_idx + 1] : "";
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

private:
  int _idx;
  int _id_arr;
  int _id_obj;
};

#endif // !PARSER
