#ifndef SCANNER
#define SCANNER

#include "./DataTypes.hpp"
#include <fstream>
#include <regex>
#include <string>

class Scanner {
public:
  Scanner() = default;
  Scanner(Scanner &&) = default;
  Scanner &operator=(Scanner &&) = default;
  ~Scanner() = default;

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
            Datas::tokens.push_back(temp);
            temp.clear();
          }
          if (isSeparator(c)) {
            Datas::tokens.push_back(std::string(1, c));
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

private:
  std::ifstream _ifs;
  std::string _buffer;
};

#endif // !SCANNER
