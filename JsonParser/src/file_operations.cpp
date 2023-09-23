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

  _ifs.close();
  // printf("%s\n", _buffer.c_str());
}

void JsonParser::writeToFile(std::string file_path) {
  _ofs.open(file_path, _ifs.out);

  if (!_ofs.is_open()) {
    printf("failed to open %s!\n", file_path.c_str());
    exit(1);
  }

  _ofs << _buffer;

  _ofs.close();
}

void JsonParser::writeToFile() {
  _ofs.open(_file_path, _ifs.out);

  if (!_ofs.is_open()) {
    printf("failed to open %s!\n", _file_path.c_str());
    exit(1);
  }

  _ofs << _buffer;

  _ofs.close();
}
