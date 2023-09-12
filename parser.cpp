#include "./parser.hpp"

bool Parser::readNumber(const std::string number) {
  std::regex number_regex(
      R"(-?(0|[1-9][0-9]*)(\.[0-9]+)?((e|E)[-+]?[0-9]+)?)");
  if (std::regex_match(number, number_regex)) {
    return true;
  }
  return false;
}

bool Parser::readString() {
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

bool Parser::readKey() { return readString(); }
bool Parser::readValue() { return false; }

bool Parser::readObject() {
  Datas::Value t_obj._val._obj = Datas::Object();

  std::string token = Peek();

  while (token != "}") {
    if (readKey()) {
      _idx++;
    }
    token = Peek();
    if (token == ":") {
      _idx++;
      if (readValue()) {
        values.push_back(t_obj)
        return true;
      }
    } else {
      return false;
    }
  }
  return false;
}

bool Parser::parse() {
  std::string token = Peek();
  if (token == "{") {
    _idx++;
    readObject();
  }
  return false;
}
