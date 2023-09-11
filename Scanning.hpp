#ifndef SCANNING
#define SCANNING

#include "./DataTypes.hpp"
#include <fstream>

class Scanning {
public:
  Scanning();
  Scanning(Scanning &&) = default;
  Scanning(const Scanning &) = default;
  Scanning &operator=(Scanning &&) = default;
  Scanning &operator=(const Scanning &) = default;
  ~Scanning();

  void readFromFile(std::string file_path) {
    ifs.open(file_path, ifs.in);

    if (!ifs.is_open()) {
      printf("failed to open %s!\n", file_path.c_str());
      exit(1);
    }
    
    std::string line;
    while (ifs >> line) {
      buffer += line;
    }

    printf("buffer: %s\n", buffer.c_str());

  }

private:
  ifstream ifs;
  std::string buffer;
};

Scanning::Scanning() {
}

Scanning::~Scanning() {
}

#endif // !SCANNING
