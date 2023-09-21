#include "../include/json_parser.hpp"

void JsonParser::readFromFile(std::string file_path) {
  _file_path = file_path;

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


