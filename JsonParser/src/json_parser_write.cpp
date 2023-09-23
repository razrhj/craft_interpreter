#include "../include/json_parser.hpp"
#include <sstream>

std::string JsonParser::writeNumber(const double num) {
  std::string temp;
  std::stringstream ss;
  ss << num;
  ss >> temp;
  return temp;
}

std::string JsonParser::writeSpecialLiteral(const std::string sl) {
  std::string temp(sl);
  return temp;
}

std::string JsonParser::writeString(const std::string str) {
  std::string temp(str);
  return temp;
}

std::string JsonParser::writeKey(const std::string key) {
  std::string temp(key);
  return temp;
}

std::string JsonParser::writeObject(const DataTypes::Object &obj) {
  std::string temp;

  auto object = obj._key_val;
  temp.append("{");

  for (auto it = object.begin(); it != object.end(); ++it) {
    temp.append(writeKey(it->first));
    temp.append(":");
    temp.append(writeValue(*(it->second)));
    temp.append(",");
  }

  temp.pop_back();

  temp.append("}");
  return temp;
}

std::string JsonParser::writeArray(const DataTypes::Array &arr) {
  std::string temp;

  auto array = arr._vals;

  temp.append("[");

  for (auto rit = array.rbegin(); rit != array.rend(); ++rit) {
    temp.append(writeValue(**rit));
    temp.append(",");
  }

  temp.pop_back();

  temp.append("]");
  return temp;
}

std::string JsonParser::writeValue(const DataTypes::Value &val) {
  std::string temp;

  switch (val._type_id) {

  case DataTypes::Init:
  case DataTypes::OBJECT: {
    auto obj = std::get<DataTypes::Object>(val._val);
    temp.append(writeObject(obj));
  } break;
  case DataTypes::ARRAY: {
    auto arr = std::get<DataTypes::Array>(val._val);
    temp.append(writeArray(arr));
  } break;
  case DataTypes::KEY: {
    auto key = std::get<std::string>(val._val);
    temp.append(writeKey(key));
  } break;
  case DataTypes::STRING: {
    auto str = std::get<std::string>(val._val);
    temp.append(writeString(str));
  } break;
  case DataTypes::True:
  case DataTypes::False:
  case DataTypes::Null: {
    auto sl = std::get<std::string>(val._val);
    temp.append(writeSpecialLiteral(sl));
  } break;
  case DataTypes::NUMBER: {
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

std::string JsonParser::write() {
  _buffer.clear();
  _buffer.append(writeValue(_val));
  writeToFile();
  return _buffer;
}

std::string JsonParser::write(const std::string file_path) {
  _buffer.clear();
  _buffer.append(writeValue(_val));
  writeToFile(file_path);
  return _buffer;
}
