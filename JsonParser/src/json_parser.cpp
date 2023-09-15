#include "../include/json_parser.hpp"
#include <cstdio>
#include <string>

DataTypes::Value *JsonParser::readNumber(const std::string path) {

  DataTypes::Value *t_num = new DataTypes::Value(DataTypes::NUMBER, path);

  // std::regex
  // number_regex(R"(-?(0|[1-9][0-9]*)(\.[0-9]+)?((e|E)[-+]?[0-9]+)?)");

  std::regex int_regex(R"(-?(0|[1-9][0-9]*))");

  std::regex float_regex(
      R"(-?(0|[1-9][0-9]*)((\.[0-9]+)|(e|E[-+]?[0-9]+)|(\.[0-9]+(e|E)[-+]?[0-9]+)))");

  std::string token = Peek();
  // printf("number: %s\n", token.c_str());

  if (std::regex_match(token, int_regex)) {
    t_num->_inum = std::stoi(token);
    _values[token][path] = t_num;
    idxForward();
    return t_num;
  }
  if (std::regex_match(token, float_regex)) {
    t_num->_fnum = std::stod(token);
    _values[token][path] = t_num;
    idxForward();
    return t_num;
  }

  // printf("not a number\n");
  return nullptr;
}

DataTypes::Value *JsonParser::readSpecialLiteral(const std::string path) {
  DataTypes::Value *t_sl = new DataTypes::Value(path);

  std::string token = Peek();
  // printf("special literal: %s\n", token.c_str());

  if (token == "true") {
    t_sl->_type_id = DataTypes::True;
  }
  if (token == "false") {
    t_sl->_type_id = DataTypes::False;
  }
  if (token == "null") {
    t_sl->_type_id = DataTypes::Null;
  }

  t_sl->_path += token;

  _values[token][path] = t_sl;
  idxForward();

  return t_sl;
}

DataTypes::Value *JsonParser::readString(const std::string path) {

  DataTypes::Value *t_str = new DataTypes::Value(DataTypes::STRING, path);

  std::string token = Peek();
  // printf("string: %s\n", token.c_str());
  // std::regex string_regex(
  //     R"("([^\"\\]|(\\(\\|\"|\/|\b|\r|\n|\f|\t|(\u[0-9a-fA-F]{4}))))*")");
  std::regex string_regex(R"("([^\"\\]|(\\(\\|\"|\/|\b|\r|\n|\f|\t)))*")");
  if (std::regex_match(token, string_regex)) {
    t_str->_str_or_bool_or_null = token;
    _values[token][path] = t_str;
    ;
    idxForward();
    return t_str;
  }
  // printf("not a string\n");
  return nullptr;

  // if (token[0] == '"') {
  //   idxforward();
  // } else {
  //   return false;
  // }
  //
  // datatypes::value *flag = true;
  // for (int i = 0; i < token.size(); ++i) {
  //   if (token[i] == '\\') {
  //     flag = !flag;
  //   }
  //   if (token[i] == '"') {
  //     if (i == token.size() - 1) {
  //       return flag;
  //     } else {
  //       return !flag;
  //     }
  //   }
  // }
  // return true;
}

DataTypes::Value *JsonParser::readKey(const std::string path) {
  return readString(path);
}

DataTypes::Value *JsonParser::readValue(const std::string path) {

  std::string token = Peek();
  // printf("readValue: %s\n", token.c_str());

  if (token == "{") {
    idxForward();
    return readObject(path);
  }
  if (token == "[") {
    idxForward();
    return readArray(path);
  }
  if (token == "true" || token == "false" || token == "null") {
    return readSpecialLiteral(path);
  }
  if (token[0] == '"') {
    return readString(path);
  }
  if (token[0] == '-' || (token[0] >= '0' && token[0] <= '9')) {
    return readNumber(path);
  }

  // printf("path: %s\n", path.c_str());
  // printf("value: expected a value\n");
  return nullptr;
}

DataTypes::Value *JsonParser::readObject(const std::string path) {
  std::string cur_obj_id = "object" + std::to_string(++_id_obj);
  std::string local_path = path + cur_obj_id;

  DataTypes::Value *t_obj = new DataTypes::Value(DataTypes::OBJECT, local_path);

  std::string token = Peek();
  // printf("readObject: %s\n", token.c_str());

  while (token != "}") {
    token = Peek();
    // printf("readObject: %s\n", token.c_str());
    std::string key;
    if (readKey(local_path + "/")) {
      key = token;

      token = Peek();
      // printf("readObject: %s\n", token.c_str());

      if (token == ":") {
        idxForward();
        DataTypes::Value *val = readValue(local_path + "/");
        if (val) {
          t_obj->_obj._key_val[key] = val;

          token = Peek();
          // printf("readObject: %s\n", token.c_str());

          if (token == ",") {
            idxForward();

          } else if (token == "}") {

            idxForward();

            _values[cur_obj_id][path] = t_obj;
            // _id_obj++;
            return t_obj;

          } else {
            // printf("object: expected \",\"\n");
            return nullptr;
          }
        } else {
          // printf("object: expected: a value\n");
        }
      } else {
        // printf("object: expected a \":\"\n");
        return nullptr;
      }
    } else {
      // printf("object: expected a key\n");
      return nullptr;
    }
  }

  idxForward();

  _values[cur_obj_id][path] = t_obj;
  // _id_obj++;

  return t_obj;
}

DataTypes::Value *JsonParser::readArray(const std::string path) {
  std::string cur_arr_id = "array" + std::to_string(++_id_arr);
  std::string local_path = path + cur_arr_id;

  DataTypes::Value *t_arr = new DataTypes::Value(DataTypes::ARRAY, local_path);

  std::string token = Peek();
  // printf("readArray: %s\n", token.c_str());

  while (token != "]") {
    token = Peek();
    // printf("readArray: %s\n", token.c_str());
    DataTypes::Value *val = readValue(local_path + "/");
    t_arr->_arr._vals.push_back(val);

    token = Peek();
    // printf("readArray: %s\n", token.c_str());
    if (token == ",") {
      idxForward();

    } else if (token == "]") {

      idxForward();

      _values[cur_arr_id][path] = t_arr;
      // _id_arr++;

      return t_arr;

    } else {
      // printf("array: expected a \",\"\n");
      return nullptr;
    }
  }

  idxForward();

  _values[cur_arr_id][path] = t_arr;
  // _id_arr++;

  return t_arr;
}

DataTypes::Value *JsonParser::parse() { return readValue("/"); }
