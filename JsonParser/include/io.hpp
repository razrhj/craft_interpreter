#ifndef IN_AND_OUT
#define IN_AND_OUT

#include "jsondatas.hpp"
#include <fstream>
#include <string>

class IO {
private:
  std::ifstream _ifs;
  std::ofstream _ofs;
  std::string _file_path;
  std::string _buffer;

public:
  IO() = default;
  IO(IO &&) = default;
  IO(const IO &) = delete;
  IO &operator=(IO &&) = default;
  IO &operator=(const IO &) = delete;
  ~IO() = default;

  // Scan
  std::string &readJson(std::string file_path);

  std::string &readJson(std::istream &in);

  // Write
  void writeToFile(std::string file_path);

  void writeToFile();

  std::string write(const jsondatas::Value &_val);

  std::string write(const jsondatas::Value &_val, std::string file_path);

  std::string writeNumber(const double num);

  std::string writeSpecialLiteral(const std::string sl);

  std::string writeString(const std::string str);

  std::string writeKey(const std::string key);

  std::string writeValue(const jsondatas::Value &val);

  std::string writeObject(const jsondatas::Object &obj);

  std::string writeArray(const jsondatas::Array &arr);
};

#endif // !IN_AND_OUT
