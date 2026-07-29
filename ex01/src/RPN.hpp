#ifndef RPN_HPP
#define RPN_HPP

#include <stack>
#include <string>

class RPN {

private:
  std::stack<int> _stack;
  int applyOp(int a, int b, char op);

public:
  RPN();
  ~RPN();
  RPN(const RPN &);
  RPN &operator=(const RPN &);

  void evaluate(const std::string &expr);
};

#endif