#include "./parser.hpp"
#include "DataTypes.hpp"
#include <string>

DataTypes::Value *Parser::readNumber(const std::string path) {

  DataTypes::Value *t_num = new DataTypes::Value(DataTypes::NUMBER, path);

  // std::regex
  // number_regex(R"(-?(0|[1-9][0-9]*)(\.[0-9]+)?((e|E)[-+]?[0-9]+)?)");

  std::regex int_regex(R"(-?([1-9][0-9]*))");

  std::regex float_regex(
      R"(-?(0|[1-9][0-9]*)((\.[0-9]+)|(e|E[-+]?[0-9]+)|(\.[0-9]+(e|E)[-+]?[0-9]+)))");

  std::string token = Peek();

  if (std::regex_match(token, int_regex)) {
    t_num->_inum = std::stoi(token);
    Datas::values[path + token] = t_num;
    return t_num;
  }
  if (std::regex_match(token, float_regex)) {
    t_num->_fnum = std::stod(token);
    Datas::values[path + token] = t_num;
    return t_num;
  }
  return nullptr;
}

DataTypes::Value *Parser::readSpecialLiteral(const std::string path) {
  DataTypes::Value *t_sl = new DataTypes::Value(path);

  std::string token = Peek();

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

  Datas::values[path + token] = t_sl;

  return t_sl;
}

DataTypes::Value *Parser::readString(const std::string path) {

  DataTypes::Value *t_str = new DataTypes::Value(DataTypes::STRING, path);

  std::string token = Peek();
  // std::regex string_regex(
  //     R"("([^\"\\]|(\\(\\|\"|\/|\b|\r|\n|\f|\t|(\u[0-9a-fA-F]{4}))))*")");
  std::regex string_regex(R"("([^\"\\]|(\\(\\|\"|\/|\b|\r|\n|\f|\t)))*")");
  if (std::regex_match(token, string_regex)) {
    t_str->_str_or_bool_or_null = token;
    Datas::values[path + token] = t_str;
    return t_str;
  }
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

DataTypes::Value *Parser::readKey(const std::string path) {
  return readString(path);
}

DataTypes::Value *Parser::readValue(const std::string path) {

  std::string token = Peek();

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

  return nullptr;
}

DataTypes::Value *Parser::readObject(const std::string path) {
  std::string cur_obj_id = "object" + std::to_string(_id_obj + 1);
  std::string local_path = path + cur_obj_id;

  DataTypes::Value *t_obj = new DataTypes::Value(DataTypes::OBJECT, local_path);

  std::string token = Peek();

  while (token != "}") {
    std::string key;
    if (readKey(local_path + "/")) {
      idxForward();
      key = token;
    }

    token = Peek();

    if (token == ":") {
      idxForward();
      DataTypes::Value *val = readValue(local_path + "/");
      if (val) {
        t_obj->_obj._key_val[key] = val;
      }
    } else {
      return nullptr;
    }
  }

  Datas::values[cur_obj_id] = t_obj;
  _id_obj++;

  return t_obj;
}

DataTypes::Value *Parser::readArray(const std::string path) {
  std::string cur_arr_id = "array" + std::to_string(_id_arr + 1);
  std::string local_path = path + cur_arr_id;

  DataTypes::Value *t_arr = new DataTypes::Value(DataTypes::ARRAY, local_path);

  std::string token = Peek();

  while (token != "]") {
    DataTypes::Value *val = readValue(local_path + "/");
    t_arr->_arr._vals.push_back(val);
    if (token == ",") {
      readValue(local_path + "/");
      t_arr->_arr._vals.push_back(val);
    }
  }

  Datas::values[cur_arr_id] = t_arr;
  _id_arr++;

  return t_arr;
}

DataTypes::Value *Parser::parse() { return readValue("/"); }
