#ifndef DATA_TYPES
#define DATA_TYPES

#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace DataTypes {

enum Type { Init, OBJECT, ARRAY, KEY, STRING, True, False, Null, NUMBER };

inline std::string typeToString(const Type type) {
  std::string ret = "NONE";

  switch (type) {

  case Init:
    ret = "Init";
    break;
  case OBJECT:
    ret = "OBJECT";
    break;
  case ARRAY:
    ret = "ARRAY";
    break;
  case KEY:
    ret = "KEY";
    break;
  case STRING:
    ret = "STRING";
    break;
  case True:
    ret = "True";
    break;
  case False:
    ret = "False";
    break;
  case Null:
    ret = "Null";
    break;
  case NUMBER:
    ret = "NUMBER";
    break;
  }
  return ret;
}

struct Object;
struct Array;
struct Value;

typedef struct Object {
  std::unordered_map<std::string, std::shared_ptr<Value>> _key_val;

  std::shared_ptr<Value> const operator[](const std::string str) {
    return _key_val[str];
  }
} Object;

typedef struct Array {
  std::vector<std::shared_ptr<Value>> _vals;

  std::shared_ptr<Value> const operator[](const int i) {
    return _vals[i];
  }
} Array;

typedef struct Value {
  Type _type_id;
  std::string _path;

  std::variant<Object, Array, std::string, int, double> _val;

  Value() {
    _type_id = Init;
    _path = "";
    _val = 14250;
  }

  Value(const Type type, const std::string path) : _type_id(type), _path(path) {
    switch (type) {

    case Init:
    case OBJECT:
      _val = Object();
      break;
    case ARRAY:
      _val = Array();
    case KEY:
    case STRING:
    case True:
    case False:
    case Null:
      _val = "";
      break;
    case NUMBER:
      _val = -1;
      break;
    }
  }

  Value operator[](const int i) {
    if (auto ptr = std::get_if<Array>(&_val)) {
      return *((*ptr)[i]);
    }
    return Value();
  }

  Value operator[](const std::string str) {
    if (auto ptr = std::get_if<Object>(&_val)) {
      return *((*ptr)[str]);
    }
    return Value();
  }

  Object getObjectValue() const {
    if (_type_id == Type::STRING) {
      return std::get<Object>(_val);
    }
    return Object();
  }

  Array getArrayValue() const {
    if (_type_id == Type::ARRAY) {
      return std::get<Array>(_val);
    }
    return Array();
  }

  std::string getStringValue() const {
    if (_type_id == Type::STRING) {
      return std::get<std::string>(_val);
    }
    return "";
  }

  int getIntValue() const {
    if (_type_id == Type::NUMBER) {
      if (auto ptr = std::get_if<int>(&_val)) {
        return *ptr;
      }
    }
    return -1;
  }

  double getDoubleValue() const {
    if (_type_id == Type::NUMBER) {
      if (auto ptr = std::get_if<double>(&_val)) {
        return *ptr;
      }
    }
    return -2.0;
  }

} _value;

} // namespace DataTypes

#endif // !DATA_TYPES
