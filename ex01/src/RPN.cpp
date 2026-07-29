#include "RPN.hpp"
#include <iostream>
#include <stdexcept>

RPN::RPN() {}
RPN::~RPN() {}
RPN::RPN(const RPN &other) : _stack(other._stack) {}
RPN &RPN::operator=(const RPN &other) {
  if (this != &other) {
    _stack = other._stack;
  }
  return *this;
}

int RPN::applyOp(int a, int b, char op) {

  switch (op) {
  case '+':
    return a + b;
  case '-':
    return a - b;
  case '*':
    return a * b;
  case '/':
    if (b == 0)
      throw std::runtime_error("Division by zero");
    return a / b;
  default:
    throw std::runtime_error("Invalid operator");
  }
}

void RPN::evaluate(const std::string &expr) {
  for (size_t i = 0; i < expr.length(); i++) {
    if (expr[i] == ' ') {
      continue;
    } else if (isdigit(expr[i])) {
      _stack.push(expr[i] - '0');
    } else {
      if (_stack.size() < 2)
        throw std::runtime_error("Invalid expression");
      int a = _stack.top();
      _stack.pop();
      int b = _stack.top();
      _stack.pop();
      _stack.push(applyOp(b, a, expr[i]));
    }
  }
  if (_stack.size() != 1)
    throw std::runtime_error("Invalid expression");
  std::cout << _stack.top() << std::endl;
}