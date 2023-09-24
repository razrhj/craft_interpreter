#include "../include/scanner.hpp"
#include <vector>

bool Scanner::isBlank(char c) {
  return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' ||
         c == '\b' || c == '\f';
}

bool Scanner::isSeparator(char c) {
  return c == '{' || c == '}' || c == '[' || c == ']' || c == ':' || c == ',';
}

std::vector<std::string> &Scanner::scanBuffer(const std::string &_buffer) {
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

  return _tokens;
}
