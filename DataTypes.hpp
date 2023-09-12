#ifndef DATA_TYPES
#define DATA_TYPES

#include <string>
#include <vector>
namespace DataTypes {

  enum Type {
    Init,
    Object,
    Array,
    String,
    Number,
    Key,
    Value,
    true,
    false,
    null
  };

  struct Object;
  struct Array;
  struct Value;

  struct Object {
    std::unordered_map<std::string, Value*> _key_val;
  };

  struct Array {
    std::vector<Value*> _vals;
  };

  struct Value {
    Type _type_id;

    union _val {
      Object _obj;
      Array _arr;
      std::string _str_or_bool_or_null;
      double _fnum;
      int _inum;
    };
  };

}

namespace Datas {
  std::vector<std::string> tokens;
}

#endif // !DATA_TYPES
