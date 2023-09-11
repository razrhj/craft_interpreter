#ifndef DATA_TYPES
#define DATA_TYPES

#include <string>
#include <vector>
namespace DataTypes {

  struct Object;
  struct Array;
  struct Value;

  struct Object {
    std::string _key;
    Value _val;
  };
  
  struct Array {
    std::vector<Value> _vals; };
  struct Value {
  
    int _type_id;
    union _val {
      Object _obj;
      Array _arr;
      std::string _str_or_bool_or_null;
      double _fnum;
      int _inum;
    };
    
  };
}

#endif // !DATA_TYPES
