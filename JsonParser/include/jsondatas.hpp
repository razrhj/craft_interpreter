#ifndef JSON_DATAS
#define JSON_DATAS

#include <cstdio>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace jsondatas {

const std::shared_ptr<std::string> null = nullptr;

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

  std::shared_ptr<Value> &operator[](const std::string key) {
    return _key_val[key];
  }
} Object;

typedef struct Array {
  std::vector<std::shared_ptr<Value>> _vals;

  std::shared_ptr<Value> &operator[](const int i) { return _vals[i]; }

} Array;

typedef struct Value {
  Type _type_id;
  std::string _path;

  std::variant<Object, Array, std::string, int, double, Type> _val;

  Value() {
    _type_id = Init;
    _path = "";
    _val = Type::Init;
  }

  Value(const Type type, const std::string path) : _type_id(type), _path(path) {
    switch (type) {

    case Init:
    case OBJECT:
      _val = Object();
      break;
    case ARRAY:
      _val = Array();
      break;
    case KEY:
    case STRING:
    case True:
    case False:
    case Null:
      _val = "";
      break;
    case NUMBER:
      _val = 14250.1425014250;
      break;
    default:
      _val = Type::Init;
      break;
    }
  }

  Value &setObject() {
    if (_type_id != Init) {
      printf("Warning: the operation will overwrite the value, the original "
             "value will be replaced!\n");
    }

    _type_id = Type::OBJECT;
    _val = Object();

    return *this;
  }

  Value &setArray() {
    if (_type_id != Init) {
      printf("Warning: the operation will overwrite the value, the original "
             "value will be replaced!\n");
    }

    _type_id = Type::ARRAY;
    _val = Array();

    return *this;
  }

  Value &setString() {
    if (_type_id != Init) {
      printf("Warning: the operation will overwrite the value, the original "
             "value will be replaced!\n");
    }

    _type_id = Type::STRING;
    _val = "";

    return *this;
  }

  Value &setKey() {
    if (_type_id != Init) {
      printf("Warning: the operation will overwrite the value, the original "
             "value will be replaced!\n");
    }

    _type_id = Type::KEY;
    _val = "";

    return *this;
  }

  Value &setNumber() {
    if (_type_id != Init) {
      printf("Warning: the operation will overwrite the value, the original "
             "value will be replaced!\n");
    }

    _type_id = Type::NUMBER;
    _val = 14250.1425014250;

    return *this;
  }

  Value &setTrue() {
    if (_type_id != Init) {
      printf("Warning: the operation will overwrite the value, the original "
             "value will be replaced!\n");
    }

    _type_id = Type::True;
    _val = "true";

    return *this;
  }

  Value &setFalse() {
    if (_type_id != Init) {
      printf("Warning: the operation will overwrite the value, the original "
             "value will be replaced!\n");
    }

    _type_id = Type::False;
    _val = "false";

    return *this;
  }

  Value &setNull() {
    if (_type_id != Init) {
      printf("Warning: the operation will overwrite the value, the original "
             "value will be replaced!\n");
    }

    _type_id = Type::Null;
    _val = "null";

    return *this;
  }

  Value &operator[](const std::string str) {
    const std::string key = "\"" + str + "\"";

    if (auto ptr = std::get_if<Object>(&_val)) {
      if (!ptr->_key_val.count(key)) {
        ptr->_key_val.insert({key, std::shared_ptr<Value>(new Value())});
      }
      return *((*ptr)[key]);
    }

    printf("Current value is not an Object-type!\n");
    exit(3);

    // return *(std::get<Object>(_val)[key]);
    // if (auto ptr = std::get_if<Object>(&_val)) {
    //   return *((*ptr)[str]);
    // }
    // return Value();
  }

  Value &operator[](const int i) {
    if (auto ptr = std::get_if<Array>(&_val)) {
      if (i >= ptr->_vals.size()) {
        ptr->_vals.push_back(std::shared_ptr<Value>(new Value()));
      }
      return *((*ptr)[i]);
    }

    printf("Current value is not an Array-type!\n");
    exit(3);

    // return *(std::get<Array>(_val)[i]);
    // if (auto ptr = std::get_if<Array>(&_val)) {
    //   return *((*ptr)[i]);
    // }
    // return Value();
  }

  Value &operator=(const bool b) {
    if (b) {
      _val = "true";
      _type_id = Type::True;

    } else {
      _val = "false";
      _type_id = Type::False;
    }

    return *this;
  }

  Value &operator=(const std::string *p) {
    _val = "null";
    _type_id = Type::Null;
    return *this;
  }

  Value &operator=(const std::string str) {
    _val = "\"" + str + "\"";
    _type_id = Type::STRING;
    return *this;
  }

  Value &operator=(const char *str) {
    _val = "\"" + std::string(str) + "\"";
    _type_id = Type::STRING;
    return *this;
  }

  Value &operator=(const int num) {
    _val = num;
    _type_id = Type::NUMBER;
    return *this;
  }

  Value &operator=(const double num) {
    _val = num;
    _type_id = Type::NUMBER;
    return *this;
  }

  Value &operator=(const Object &obj) {
    _val = obj;
    _type_id = Type::OBJECT;
    return *this;
  }

  Value &operator=(const Array &arr) {
    _val = arr;
    _type_id = Type::ARRAY;
    return *this;
  }

  Object &getObjectValue() {
    return std::get<Object>(_val);
    // if (_type_id == Type::STRING) {
    //   return std::get<Object>(_val);
    // }
    // return Object();
  }

  Array &getArrayValue() {
    return std::get<Array>(_val);
    // if (_type_id == Type::ARRAY) {
    //   return std::get<Array>(_val);
    // }
    // return Array();
  }

  std::string &getStringValue() {
    return std::get<std::string>(_val);
    // if (_type_id == Type::STRING || _type_id == Type::KEY ||
    //     _type_id == Type::True || _type_id == Type::False ||
    //     _type_id == Type::Null) {
    //   return std::get<std::string>(_val);
    // }
    // return "";
  }

  int &getIntValue() {
    return std::get<int>(_val);
    // if (_type_id == Type::NUMBER) {
    //   if (auto ptr = std::get_if<int>(&_val)) {
    //     return *ptr;
    //   }
    // }
    // return -1;
  }

  double &getDoubleValue() {
    return std::get<double>(_val);
    // if (_type_id == Type::NUMBER) {
    //   if (auto ptr = std::get_if<double>(&_val)) {
    //     return *ptr;
    //   }
    // }
    // return -2.0;
  }

} _value;

} // namespace jsondatas

#endif // !JSON_DATAS
