/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 18:00:48 by carlopez          #+#    #+#             */
/*   Updated: 2026/08/01 18:00:50 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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