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

  bool readNumber(std::string number) {
    std::regex number_regex(
        R"(-?(0|[1-9][0-9]*)(\.[0-9]+)?((e|E)[-+]?[0-9]+)?)");
    if (std::regex_match(number, number_regex)) {
      return true;
    }
    return false;
  }

  bool readString() {
    std::string token = Peek();
    // std::regex string_regex(
    //     R"("([^\"\\]|(\\(\\|\"|\/|\b|\r|\n|\f|\t|(\u[0-9a-fA-F]{4}))))*")");
    std::regex string_regex(R"("([^\"\\]|(\\(\\|\"|\/|\b|\r|\n|\f|\t)))*")");
    return regex_match(token, string_regex);

    if (token[0] == '"') {
      _idx++;
    } else {
      return false;
    }

    bool flag = true;
    for (int i = 0; i < token.size(); ++i) {
      if (token[i] == '\\') {
        flag = !flag;
      }
      if (token[i] == '"') {
        if (i == token.size() - 1) {
          return flag;
        } else {
          return !flag;
        }
      }
    }
    return true;
  }

  bool readKey() { return readString(); }
  bool readValue() { return false; }

  bool readObject() {
    std::string token = Peek();
    while (token != "}") {
      if (readKey()) {
        _idx++;
      }
      token = Peek();
      if (token == ":") {
        _idx++;
        if (readValue()) {
          return true;
        }
      } else {
        return false;
      }
    }
    return false;
  }

  bool parse() {
    std::string token = Peek();
    if (token == "{") {
      _idx++;
      readObject();
    }
    return false;
  }

private:
  int _idx;
};
