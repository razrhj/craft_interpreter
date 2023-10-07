#pragma once
#include "Expr.hpp"
#include "Token.hpp"
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

template <typename R> class AstPrinter : public Visitor<R> {
public:
  AstPrinter() = default;
  AstPrinter(AstPrinter &&) = default;
  AstPrinter(const AstPrinter &) = default;
  AstPrinter &operator=(AstPrinter &&) = default;
  AstPrinter &operator=(const AstPrinter &) = default;
  ~AstPrinter() = default;

  void run(int argc, char *argv[]) {
    std::shared_ptr<Expr<R>> expression = std::make_shared<Binary<R>>(
        std::make_shared<Unary<R>>(Token(TokenType::MINUS, "-", Object(TokenType::NONE), 1),
                     std::make_shared<Literal<R>>(Object(123))),
        Token(TokenType::STAR, "*", Object(TokenType::NONE), 1),
        std::make_shared<Grouping<R>>(std::make_shared<Literal<R>>(Object(45.67))));

    std::cout << AstPrinter<R>().print(expression);
  }

  R print(const std::shared_ptr<Expr<R>> expr) const {
    return expr->accept(*this);
  }

  R visitBinaryExpr(const Binary<R> &expr) const override {
    return parenthesize(expr.oprtor.getLexeme(), expr.left, expr.right);
  }

  R visitGroupingExpr(const Grouping<R> &expr) const override {
    return parenthesize("group", expr.expression);
  }

  R visitLiteralExpr(const Literal<R> &expr) const override {
    if (std::get_if<TokenType>(&expr.value)) {
      return "nil";
    }
    return ObjectToString(expr.value);
  }

  R visitUnaryExpr(const Unary<R> &expr) const override {
    return parenthesize(expr.oprtor.getLexeme(), expr.right);
  }

  template <typename... Exprs>
  std::string processParenthesize(std::shared_ptr<Exprs>... exprs) const {
    std::string result;
    (..., (result += " " + exprs->accept(*this)));
    return result;
  }

  template <typename... Exprs>
  R parenthesize(const std::string &name,
                 const std::shared_ptr<Exprs>... exprs) const {
    return "(" + name + processParenthesize(exprs...) + ")";
  }

private:
};
