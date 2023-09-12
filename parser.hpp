#include "./DataTypes.hpp"
#include <regex>

class Parser {
public:
  Parser() = default;
  Parser(Parser &&) = default;
  Parser(const Parser &) = default;
  Parser &operator=(Parser &&) = default;
  Parser &operator=(const Parser &) = default;
  ~Parser() = default;

  std::string Peek() { return Datas::tokens[_idx]; }

  bool readNumber(const std::string number);

  bool readString();

  bool readKey();

  bool readValue();

  bool readObject();

  bool parse();

private:
  int _idx;
  std::unordered_map<Datas::Value, Datas::Type> values;
};
