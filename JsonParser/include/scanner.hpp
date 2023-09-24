#ifndef SCANNER
#define SCANNER

#include "jsondatas.hpp"
#include <string>
#include <vector>

class Scanner {
private:
  std::vector<std::string> _tokens;

public:
  Scanner() = default;
  Scanner(Scanner &&) = default;
  Scanner(const Scanner &) = default;
  Scanner &operator=(Scanner &&) = default;
  Scanner &operator=(const Scanner &) = default;
  ~Scanner() = default;

  // Scan
  bool isBlank(char c);

  bool isSeparator(char c);

  std::vector<std::string> &scanBuffer(const std::string &_buffer);
};

#endif // !SCANNER
