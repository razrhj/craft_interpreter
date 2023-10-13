#include "Expr.hpp"

template <typename R> class Interpreter : public Visitor<R> {
public:
  Interpreter() = default;
  Interpreter(Interpreter &&) = default;
  Interpreter(const Interpreter &) = default;
  Interpreter &operator=(Interpreter &&) = default;
  Interpreter &operator=(const Interpreter &) = default;
  ~Interpreter() = default;

private:
};
