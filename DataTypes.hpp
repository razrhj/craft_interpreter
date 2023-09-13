#ifndef DATA_TYPES
#define DATA_TYPES

#include <string>
#include <unordered_map>
#include <vector>

namespace DataTypes {

enum Type { Init, OBJECT, ARRAY, KEY, STRING, True, False, Null, NUMBER };

struct Object;
struct Array;
struct Value;

typedef struct Object {
  std::unordered_map<std::string, struct Value *> _key_val;
} Object;

typedef struct Array {
  std::vector<struct Value *> _vals;
} Array;

typedef struct Value {
  Type _type_id;
  std::string _path;

  // union {
  Object _obj;
  Array _arr;
  std::string _str_or_bool_or_null;
  double _fnum;
  int _inum;
  // };

  Value() {
    _type_id = Init;
    _inum = -1;
    _path = "";
  }

  Value(std::string path) : _path(path) {
    _type_id = Init;
    _inum = -1;
  }

  Value(Type type, std::string path) : _type_id(type), _path(path) {
    switch (type) {

    case Init:
    case OBJECT:
      _obj = Object();
      break;
    case ARRAY:
      _arr = Array();
      break;
    case STRING:
    case True:
    case False:
    case Null:
    case KEY:
      _str_or_bool_or_null = "";
      break;
    case NUMBER:
      _inum = -1;
      break;
    default:
      break;
    }
    _inum = -1;
  }

} Value;

} // namespace DataTypes

#endif // !DATA_TYPES
