#include "RPN.hpp"
#include <iostream>

int main(int ac, char **av) {
  if (ac != 2) {
    std::cerr << "Error: Invalid arguments" << std::endl;
    return 1;
  }
  try {
    RPN rpn;
    rpn.evaluate(av[1]);
  } catch (std::exception &e) {
    std::cerr << "Error" << std::endl;
    return 1;
  }
  return 0;
}