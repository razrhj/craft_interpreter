#include <exception>
class Lox {
public:
  Lox() = default;
  Lox(Lox &&) = default;
  Lox(const Lox &) = default;
  Lox &operator=(Lox &&) = default;
  Lox &operator=(const Lox &) = default;
  ~Lox() = default;

  void main(int argc, char *argv[]) {
    try {

    } catch (const std::exception &) {
    }
  }

private:
};
