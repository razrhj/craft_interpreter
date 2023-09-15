#ifndef DATA_TYPES
#define DATA_TYPES

#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace DataTypes {

enum Type { Init, OBJECT, ARRAY, KEY, STRING, True, False, Null, NUMBER };

inline std::string getType(const Type type) {
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
} Object;

typedef struct Array {
  std::vector<std::shared_ptr<Value>> _vals;
} Array;

typedef struct Value {
  Type _type_id;
  std::string _path;

  std::variant<Object, Array, std::string, int, double> _val;

  Value() {
    _type_id = Init;
    _path = "";
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

} _value;

} // namespace DataTypes

#endif // !DATA_TYPES
