#include "../include/json_parser.hpp"
#include <cstdio>
#include <fstream>
#include <string>
#include <variant>

std::string JsonParser::Peek() {
  return _idx + 1 < _tokens.size() ? _tokens[_idx + 1] : "";
}

void JsonParser::idxForward() { _idx++; }

std::shared_ptr<jsondatas::Value>
JsonParser::parseNumber(const std::string path) {

  std::shared_ptr<jsondatas::Value> t_num(
      new jsondatas::Value(jsondatas::Type::NUMBER, path));

  // std::regex
  // number_regex(R"(-?(0|[1-9][0-9]*)(\.[0-9]+)?((e|E)[-+]?[0-9]+)?)");

  std::regex int_regex(R"(-?(0|[1-9][0-9]*))");

  std::regex float_regex(
      R"(-?(0|[1-9][0-9]*)((\.[0-9]+)|(e|E[-+]?[0-9]+)|(\.[0-9]+(e|E)[-+]?[0-9]+)))");

  std::string token = Peek();
  // printf("number: %s\n", token.c_str());

  if (std::regex_match(token, int_regex)) {
    t_num->_val = std::stoi(token);
    _values[token][path] = t_num;
    idxForward();
    return t_num;
  }
  if (std::regex_match(token, float_regex)) {
    t_num->_val = std::stod(token);
    _values[token][path] = t_num;
    idxForward();
    return t_num;
  }

  // printf("not a number\n");
  return nullptr;
}

std::shared_ptr<jsondatas::Value>
JsonParser::parseSpecialLiteral(const std::string path) {

  std::string token = Peek();
  // printf("special literal: %s\n", token.c_str());
  std::shared_ptr<jsondatas::Value> t_sl;

  if (token == "true") {
    t_sl = std::shared_ptr<jsondatas::Value>(
        new jsondatas::Value(jsondatas::Type::True, path));
    t_sl->_val = token;
  }
  if (token == "false") {
    t_sl = std::shared_ptr<jsondatas::Value>(
        new jsondatas::Value(jsondatas::Type::False, path));
    t_sl->_val = token;
  }
  if (token == "null") {
    t_sl = std::shared_ptr<jsondatas::Value>(
        new jsondatas::Value(jsondatas::Type::Null, path));
    t_sl->_val = token;
  }

  t_sl->_path += token;

  _values[token][path] = t_sl;
  idxForward();

  return t_sl;
}

std::shared_ptr<jsondatas::Value>
JsonParser::parseString(const std::string path, const jsondatas::Type type) {

  std::shared_ptr<jsondatas::Value> t_str(new jsondatas::Value(type, path));

  std::string token = Peek();
  // printf("string: %s\n", token.c_str());
  // std::regex string_regex(
  //     R"("([^\"\\]|(\\(\\|\"|\/|\b|\r|\n|\f|\t|(\u[0-9a-fA-F]{4}))))*")");
  std::regex string_regex(R"("([^\"\\]|(\\(\\|\"|\/|\b|\r|\n|\f|\t)))*")");
  if (std::regex_match(token, string_regex)) {
    t_str->_val = token;
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

std::shared_ptr<jsondatas::Value> JsonParser::parseKey(const std::string path) {
  return parseString(path, jsondatas::Type::KEY);
}

std::shared_ptr<jsondatas::Value>
JsonParser::parseValue(const std::string path) {

  std::string token = Peek();
  // printf("parseValue: %s\n", token.c_str());

  if (token == "{") {
    idxForward();
    return parseObject(path);
  }
  if (token == "[") {
    idxForward();
    return parseArray(path);
  }
  if (token == "true" || token == "false" || token == "null") {
    return parseSpecialLiteral(path);
  }
  if (token[0] == '"') {
    return parseString(path, jsondatas::Type::STRING);
  }
  if (token[0] == '-' || (token[0] >= '0' && token[0] <= '9')) {
    return parseNumber(path);
  }

  // printf("path: %s\n", path.c_str());
  // printf("value: expected a value\n");
  return nullptr;
}

std::shared_ptr<jsondatas::Value>
JsonParser::parseObject(const std::string path) {
  std::string cur_obj_id = "object" + std::to_string(++_id_obj);
  std::string local_path = path + cur_obj_id;

  std::shared_ptr<jsondatas::Value> t_obj(
      new jsondatas::Value(jsondatas::Type::OBJECT, local_path));

  std::string token = Peek();
  // printf("parseObject: %s\n", token.c_str());

  while (token != "}") {
    token = Peek();
    // printf("parseObject: %s\n", token.c_str());
    std::string key;
    if (parseKey(local_path + "/")) {
      key = token;

      token = Peek();
      // printf("parseObject: %s\n", token.c_str());

      if (token == ":") {
        idxForward();

        std::shared_ptr<jsondatas::Value> val = parseValue(local_path + "/");

        if (val) {
          if (auto obj = std::get_if<jsondatas::Object>(&(t_obj->_val))) {
            obj->_key_val[key] = val;
          }

          token = Peek();
          // printf("parseObject: %s\n", token.c_str());

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

std::shared_ptr<jsondatas::Value>
JsonParser::parseArray(const std::string path) {
  std::string cur_arr_id = "array" + std::to_string(++_id_arr);
  std::string local_path = path + cur_arr_id;

  std::shared_ptr<jsondatas::Value> t_arr(
      new jsondatas::Value(jsondatas::Type::ARRAY, local_path));

  std::string token = Peek();
  // printf("parseArray: %s\n", token.c_str());

  while (token != "]") {
    token = Peek();
    // printf("parseArray: %s\n", token.c_str());
    std::shared_ptr<jsondatas::Value> val = parseValue(local_path + "/");
    if (auto arr = std::get_if<jsondatas::Array>(&(t_arr->_val))) {
      arr->_vals.push_back(val);
    }

    token = Peek();
    // printf("parseArray: %s\n", token.c_str());
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

std::shared_ptr<jsondatas::Value>
JsonParser::parse(const std::string file_path) {
  _buffer = io.readFromFile(file_path);
  sc.scanBuffer(_buffer);
  std::shared_ptr<jsondatas::Value> ret = parseValue("/");
  if (ret) {
    _val = *ret;
  }
  return ret;
}

std::string JsonParser::getType(const std::string str, const std::string path) {
  return jsondatas::typeToString(_values[str][path]->_type_id);
}

std::string JsonParser::getType(const jsondatas::Type type) {
  return jsondatas::typeToString(type);
}
