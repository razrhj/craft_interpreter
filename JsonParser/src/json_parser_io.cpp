#include "../include/io.hpp"
#include <sstream>

std::string &IO::readJson(std::string file_path) {
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
  return _buffer;
}

std::string &IO::readJson(std::istream &in) {
  std::string line;
  while (std::getline(in, line)) {
    _buffer += line;
  }

  _ifs.close();
  // printf("%s\n", _buffer.c_str());
  return _buffer;
}

void IO::writeToFile(std::string file_path) {
  _ofs.open(file_path, _ifs.out);

  if (!_ofs.is_open()) {
    printf("failed to open %s!\n", file_path.c_str());
    exit(1);
  }

  _ofs << _buffer;

  _ofs.close();
}

void IO::writeToFile() {
  _ofs.open(_file_path, _ifs.out);

  if (!_ofs.is_open()) {
    printf("failed to open %s!\n", _file_path.c_str());
    exit(1);
  }

  _ofs << _buffer;

  _ofs.close();
}

std::string IO::writeNumber(const double num) {
  std::string temp;
  std::stringstream ss;
  ss << num;
  ss >> temp;
  return temp;
}

std::string IO::writeSpecialLiteral(const std::string sl) {
  std::string temp(sl);
  return temp;
}

std::string IO::writeString(const std::string str) {
  std::string temp(str);
  return temp;
}

std::string IO::writeKey(const std::string key) {
  std::string temp(key);
  return temp;
}

std::string IO::writeObject(const jsondatas::Object &obj) {
  std::string temp;

  auto object = obj._key_val;

  for (auto it = object.begin(); it != object.end(); ++it) {
    std::string key_value;
    key_value.append(writeKey(it->first));
    key_value.append(":");
    key_value.append(writeValue(*(it->second)));
    key_value.append(",");
    temp = key_value + temp;
  }

  if (temp.size()) {
    temp.pop_back();
  }

  temp = "{" + temp + "}";

  return temp;
}

std::string IO::writeArray(const jsondatas::Array &arr) {
  std::string temp;

  auto array = arr._vals;

  temp.append("[");

  for (auto it = array.begin(); it != array.end(); ++it) {
    temp.append(writeValue(**it));
    temp.append(",");
  }

  if (temp.size() > 2) {
    temp.pop_back();
  }

  temp.append("]");
  return temp;
}

std::string IO::writeValue(const jsondatas::Value &val) {
  std::string temp;

  switch (val._type_id) {

  case jsondatas::Init:
  case jsondatas::OBJECT: {
    // auto obj = std::get<jsondatas::Object>(val._val);
    temp.append(writeObject(std::get<jsondatas::Object>(val._val)));
  } break;
  case jsondatas::ARRAY: {
    // auto arr = std::get<jsondatas::Array>(val._val);
    temp.append(writeArray(std::get<jsondatas::Array>(val._val)));
  } break;
  case jsondatas::KEY: {
    // auto key = std::get<std::string>(val._val);
    temp.append(writeKey(std::get<std::string>(val._val)));
  } break;
  case jsondatas::STRING: {
    // auto str = std::get<std::string>(val._val);
    temp.append(writeString(std::get<std::string>(val._val)));
  } break;
  case jsondatas::True:
  case jsondatas::False:
  case jsondatas::Null: {
    // auto sl = std::get<std::string>(val._val);
    temp.append(writeSpecialLiteral(std::get<std::string>(val._val)));
  } break;
  case jsondatas::NUMBER: {
    auto i = std::get_if<int>(&val._val);
    auto d = std::get_if<double>(&val._val);
    double num = -1;
    if (i) {
      num = static_cast<int>(*i);
    } else if (d) {
      num = *d;
    }

    temp.append(writeNumber(num));
  } break;
  }

  return temp;
}

std::string IO::write(const jsondatas::Value &_val) {
  _buffer.clear();
  _buffer.append(writeValue(_val));
  writeToFile();
  return _buffer;
}

std::string IO::write(const jsondatas::Value &_val,
                            const std::string file_path) {
  _buffer.clear();
  _buffer.append(writeValue(_val));
  writeToFile(file_path);
  return _buffer;
}
