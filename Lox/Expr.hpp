#pragma once
#include "Lox.hpp"
#include <bits/stdc++.h>

template <typename R> class Visitor;

template <typename R> class Expr {
public:
  virtual R accept(const Visitor<R> &visitor) const = 0;
  virtual ~Expr() = default;
};

template <typename R> class Binary : public Expr<R> {
public:
  Binary(const std::shared_ptr<Expr<R>> left, const Token oprtor,
         const std::shared_ptr<Expr<R>> right)
      : left(left), oprtor(oprtor), right(right), Expr<R>() {}

  R accept(const Visitor<R> &visitor) const override {
    return visitor.visitBinaryExpr(*this);
  }

  const std::shared_ptr<Expr<R>> left;
  const Token oprtor;
  const std::shared_ptr<Expr<R>> right;
};

template <typename R> class Grouping : public Expr<R> {
public:
  Grouping(const std::shared_ptr<Expr<R>> expression)
      : expression(expression), Expr<R>() {}

  R accept(const Visitor<R> &visitor) const override {
    return visitor.visitGroupingExpr(*this);
  }

  const std::shared_ptr<Expr<R>> expression;
};

template <typename R> class Literal : public Expr<R> {
public:
  Literal(const Object value) : value(value), Expr<R>() {}

  R accept(const Visitor<R> &visitor) const override {
    return visitor.visitLiteralExpr(*this);
  }

  const Object value;
};

template <typename R> class Unary : public Expr<R> {
public:
  Unary(const Token oprtor, const std::shared_ptr<Expr<R>> right)
      : oprtor(oprtor), right(right), Expr<R>() {}

  R accept(const Visitor<R> &visitor) const override {
    return visitor.visitUnaryExpr(*this);
  }

  const Token oprtor;
  const std::shared_ptr<Expr<R>> right;
};

template <typename R> class Visitor {
public:
  virtual R visitBinaryExpr(const Binary<R> &expr) const = 0;
  virtual R visitGroupingExpr(const Grouping<R> &expr) const = 0;
  virtual R visitLiteralExpr(const Literal<R> &expr) const = 0;
  virtual R visitUnaryExpr(const Unary<R> &expr) const = 0;
};
